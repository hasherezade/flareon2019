using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sol1
{

    class Program
    {
        public static string Base64Encode(string plainText)
        {
            return Convert.ToBase64String(Encoding.UTF8.GetBytes(plainText));
        }

        private static string getCatGenetics()
        {
            return Base64Encode("Bagel_Cannon");
        }

        private static void CatFact(byte[] s, int i, int j)
        {
            byte b = s[i];
            s[i] = s[j];
            s[j] = b;
        }

        private static byte[] InvertCosmicConstants(byte[] cat)
        {
            byte[] array = (from i in Enumerable.Range(0, 256)
                            select (byte)i).ToArray<byte>();
            int j = 0;
            int num = 0;
            while (j < 256)
            {
                num = (num + (int)cat[j % cat.Length] + (int)array[j] & 255);
                CatFact(array, j, num);
                j++;
            }
            return array;
        }

        private static void AssignFelineDesignation(byte[] cat)
        {
            byte []correctOut = new byte[]
            {
                95,
                193,
                50,
                12,
                127,
                228,
                98,
                6,
                215,
                46,
                200,
                106,
                251,
                121,
                186,
                119,
                109,
                73,
                35,
                14,
                20
            };
            byte[] s = InvertCosmicConstants(cat);
            int i = 0;
            int j = 0;
            Byte[] encodedBytes = new Byte[255];
            int indx = 0;
            foreach (byte b in correctOut)
            {
                i = (i + 1 & 255);
                j = (j + (int)s[i] & 255);
                CatFact(s, i, j);
                byte k = s[(int)(s[i] + s[j] & byte.MaxValue)];
                int c = b ^ k;
                Console.WriteLine(c);
            }
        }

        static void Main(string[] args)
        {
            string catGenetics = getCatGenetics();
            AssignFelineDesignation(Encoding.UTF8.GetBytes(catGenetics));
        }
    }
}
