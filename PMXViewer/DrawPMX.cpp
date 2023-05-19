#include "DrawPMX.h"


DrawPMX::DrawPMX(Model model) : model(model),materialCount(model.materialCount)
{
	// 頂点バッファオブジェクトの作成
	glGenBuffers(1, &vbo);
	// 頂点バッファオブジェクトのバインド
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// 頂点バッファオブジェクトにデータを転送
	glBufferData(GL_ARRAY_BUFFER, model.vertexCount * sizeof(VertexData), model.vertexData.data(), GL_STATIC_DRAW);

	vao.resize(materialCount);
	// 頂点配列オブジェクトの作成
	glGenVertexArrays(materialCount, vao.data());

	ibo.resize(materialCount);
	// インデックスバッファオブジェクトの作成
	glGenBuffers(materialCount, ibo.data());
	 
	int surfaceCount;
	int offset = 0;
	for (int i = 0; i < materialCount; i++)
	{
		// 頂点配列オブジェクトをバインド
		glBindVertexArray(vao[i]);

		// attribute変数の設定および有効化
		setAndEnableVertexAttrib();

		surfaceCount = model.materialData[i].surfaceCount;
		std::vector<GLuint> index(model.vertexIndex.begin() + offset, model.vertexIndex.begin() + offset + surfaceCount);
		offset += surfaceCount;

		// インデックスバッファオブジェクトのバインド
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);

		// インデックスバッファオブジェクトにデータを転送
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, surfaceCount * sizeof(GLuint), index.data(), GL_STATIC_DRAW);
	}
}

DrawPMX::~DrawPMX()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(materialCount, vao.data());
	glDeleteBuffers(materialCount, ibo.data());
}

void DrawPMX::setAndEnableVertexAttrib()
{
	// 頂点バッファオブジェクトをattrib変数に関連付ける
	glVertexAttribPointer(POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, position)));
	glVertexAttribPointer(NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, normal)));
	glVertexAttribPointer(UV_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, uv)));
	glVertexAttribPointer(EDGE_SCALE_INDEX, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, edgeScale)));

	// attribute変数の有効化
	glEnableVertexAttribArray(POSITION_INDEX);
	glEnableVertexAttribArray(NORMAL_INDEX);
	glEnableVertexAttribArray(UV_INDEX);
	glEnableVertexAttribArray(EDGE_SCALE_INDEX);
}

void DrawPMX::bind(int n) const
{
	glBindVertexArray(vao[n]);
}

void DrawPMX::execute()
{
	glDrawElements(GL_TRIANGLES, model.faceCount, GL_UNSIGNED_INT, 0);
}

void DrawPMX::draw(int n)
{
	bind(n);
	execute();
}

int DrawPMX::getMaterialCount() { return materialCount; }