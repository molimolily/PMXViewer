#include "DrawPMX.h"


DrawPMX::DrawPMX(Model model) : model(model),materialCount(model.materialCount)
{
	// ���_�o�b�t�@�I�u�W�F�N�g�̍쐬
	glGenBuffers(1, &vbo);
	// ���_�o�b�t�@�I�u�W�F�N�g�̃o�C���h
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// ���_�o�b�t�@�I�u�W�F�N�g�Ƀf�[�^��]��
	glBufferData(GL_ARRAY_BUFFER, model.vertexCount * sizeof(VertexData), model.vertexData.data(), GL_STATIC_DRAW);

	vao.resize(materialCount);
	// ���_�z��I�u�W�F�N�g�̍쐬
	glGenVertexArrays(materialCount, vao.data());

	ibo.resize(materialCount);
	// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�̍쐬
	glGenBuffers(materialCount, ibo.data());
	 
	int surfaceCount;
	int offset = 0;
	for (int i = 0; i < materialCount; i++)
	{
		// ���_�z��I�u�W�F�N�g���o�C���h
		glBindVertexArray(vao[i]);

		// attribute�ϐ��̐ݒ肨��їL����
		setAndEnableVertexAttrib();

		surfaceCount = model.materialData[i].surfaceCount;
		std::vector<GLuint> index(model.vertexIndex.begin() + offset, model.vertexIndex.begin() + offset + surfaceCount);
		offset += surfaceCount;

		// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�̃o�C���h
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);

		// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�Ƀf�[�^��]��
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, surfaceCount * sizeof(GLuint), index.data(), GL_STATIC_DRAW);
	}

	setTexture();
}

DrawPMX::~DrawPMX()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(materialCount, vao.data());
	glDeleteBuffers(materialCount, ibo.data());
}

void DrawPMX::setAndEnableVertexAttrib()
{
	// ���_�o�b�t�@�I�u�W�F�N�g��attrib�ϐ��Ɋ֘A�t����
	glVertexAttribPointer(POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, position)));
	glVertexAttribPointer(NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, normal)));
	glVertexAttribPointer(UV_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, uv)));
	glVertexAttribPointer(EDGE_SCALE_INDEX, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<void*>(offsetof(VertexData, edgeScale)));

	// attribute�ϐ��̗L����
	glEnableVertexAttribArray(POSITION_INDEX);
	glEnableVertexAttribArray(NORMAL_INDEX);
	glEnableVertexAttribArray(UV_INDEX);
	glEnableVertexAttribArray(EDGE_SCALE_INDEX);
}

void DrawPMX::setTexture()
{
	
	texID.resize(model.texCount);
	// �e�N�X�`��ID�̓o�^
	glGenTextures(model.texCount, texID.data());

	for (int i = 0; i < model.texCount; i++)
	{
		FIBITMAP* img = model.textureData[i];
		// �e�N�X�`����L���ɂ���
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texID[i]);
		if (img != nullptr)
		{
			int width = FreeImage_GetWidth(img);
			int height = FreeImage_GetHeight(img);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(img));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			std::cerr << "Failed to set texture: " << i << std::endl;
		}
		
		glDisable(GL_TEXTURE_2D);
		FreeImage_Unload(img);
	}

	// �e�N�X�`���̃A���o�C���h
	glBindTexture(GL_TEXTURE_2D, 0);
	
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