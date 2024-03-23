#pragma once
namespace LM
{
	class FrameBuffer
	{
	protected:
		unsigned int m_fboID;		// frame buffer object id
		unsigned int m_texID;		// texture id
		unsigned int m_rboID;		// render buffer object id
		int m_width;
		int m_height;
		unsigned char m_texIndex;

		FrameBuffer();
	public:
		FrameBuffer(int width, int height, unsigned char index=0);
		virtual ~FrameBuffer();
		void Bind();
		void Unbind();
		unsigned int GetFrameBufferID()const
		{
			return m_fboID;
		}
		unsigned int GetTextureID() const;
		/// <summary>
		/// 绑定纹理并返回纹理单元
		/// </summary>
		/// <returns></returns>
		unsigned char GetTexIndex() const;
		int GetWidth()const
		{
			return m_width;
		};
		int GetHeight()const
		{
			return m_height;
		};
	};
}