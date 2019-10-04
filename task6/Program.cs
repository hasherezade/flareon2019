using System;
using System.Drawing;
using System.IO;

namespace BMPHIDE
{
	internal class Program
	{

		public static byte a(byte b, int r)
		{
			for (int i = 0; i < r; i++)
			{
				byte b2 = (byte)((b & 128) / 128);
				b = (byte)((b * 2 & byte.MaxValue) + b2);
			}
			return b;
		}

		public static byte b(byte b, int r)
		{
			for (int i = 0; i < r; i++)
			{
				byte b2 = (byte)((b & 128) / 128);
				b = (byte)((b * 2 & byte.MaxValue) + b2);
			}
			return b;
		}

		public static byte c(byte b, int r)
		{
			for (int i = 0; i < r; i++)
			{
				byte b2 = (byte)((b & 1) * 128);
				b = (byte)((b / 2 & byte.MaxValue) + b2);
			}
			return b;
		}

		public static byte d(byte b, int r)
		{
			for (int i = 0; i < r; i++)
			{
				byte b2 = (byte)((b & 1) * 128);
				b = (byte)((b / 2 & byte.MaxValue) + b2);
			}
			return b;
		}

		public static byte e(byte b, byte k)
		{
			for (int i = 0; i < 8; i++)
			{
				bool flag = (b >> i & 1) == (k >> i & 1);
				if (flag)
				{
					b = (byte)((int)b & ~(1 << i) & 255);
				}
				else
				{
					b = (byte)((int)b | (1 << i & 255));
				}
			}
			return b;
		}

		public static byte g(int idx)
		{
			byte b = (byte)((long)(idx + 1) * (long)(0x126B6FC5));
			byte k = (byte)((idx + 2) * 0xC82C97D);
			return Program.e(b, k);
		}

		public static byte[] h(byte[] data)
		{
			byte[] array = new byte[data.Length];
			int num = 0;
			for (int i = 0; i < data.Length; i++)
			{
				int num2 = (int)Program.g(num++);
				int chunk = (int)data[i];
				
				chunk = (int)Program.e((byte)chunk, (byte)num2);
				chunk = (int)Program.a((byte)chunk, 7);

				int num4 = (int)Program.g(num++);
				chunk = (int)Program.e((byte)chunk, (byte)num4);
				chunk = (int)Program.c((byte)chunk, 3);
				array[i] = (byte)chunk;
			}
			return array;
		}

		public static void encode(Bitmap bm, byte[] data)
		{
			int num = 0;
			for (int i = 0; i < bm.Width; i++)
			{
				for (int j = 0; j < bm.Height; j++)
				{
					bool flag = num > data.Length - 1;
					if (flag)
					{
						break;
					}
					Color pixel = bm.GetPixel(i, j);
					int red = ((int)pixel.R & 0xf8) | (data[num] & 7); //last 3 bytes
					int green = ((int)pixel.G & 0xf8) | ((data[num] >> 3) & 7); //last 3 bytes
					int blue = ((int)pixel.B & 0xfc) | ((data[num] >> 6) & 3); //last 2 bytes
					Color color = Color.FromArgb(0, red, green, blue); 
					bm.SetPixel(i, j, color);
					num += 1;
				}
			}
		}

		public static byte h_decode(byte chunk, int i)
		{
			int num = i * 2;
			int num2 = (int)Program.g(num++);
			int num4 = (int)Program.g(num++);

			int chunk2 = chunk;
			chunk2 = (int)Program.b((byte)chunk2, 3);
			chunk2 = (int)Program.e((byte)chunk2, (byte)num4);
			chunk2 = (int)Program.d((byte)chunk2, 7);
			chunk2 = (int)Program.e((byte)chunk2, (byte)num2);
			return (byte)chunk2;
		}

		public static byte[] decode(Bitmap bm)
		{
			byte[] array = new byte[bm.Width * bm.Height];
			int num = 0;
			for (int i = 0; i < bm.Width; i++)
			{
				for (int j = 0; j < bm.Height; j++)
				{
					
					Color pixel = bm.GetPixel(i, j);
					int redD = ((int)pixel.R & 7);
					int greenD = ((int)pixel.G & 7) << 3;
					int blueD = ((int)pixel.B & 3) << 6;
					int full = redD | greenD | blueD;
					byte chunk = (byte)full;
					byte chunk2 =  h_decode(chunk, num);
					array[num] = chunk2;
					num++;
				}
			}
			return array;
		}

		private static void Main(string[] args)
		{
			bool is_decode = false;
			if (args.Length < 2)
			{
				Console.WriteLine("Arguments missing!");
				return;
				
			}
			if (args[1].Equals("D"))
			{
				is_decode = true;
			}

			string fullPath = Path.GetFullPath(args[0]);
			Bitmap bitmap = new Bitmap(fullPath);
			if (!is_decode)
			{
				string filename = args[2];
				string fullPath2 = Path.GetFullPath(args[1]);
				byte[] data = File.ReadAllBytes(fullPath2);
				byte[] data2 = Program.h(data);
				Program.encode(bitmap, data2);

				bitmap.Save(filename);

				Program.decode(bitmap);
				System.Console.ReadKey();
				return;
			}
			byte[] array = Program.decode(bitmap);
			File.WriteAllBytes("C:\\bmptest\\out.bmp", array);
			Console.Write("Done, press a key...");
			System.Console.ReadKey();
		}
	}
}
