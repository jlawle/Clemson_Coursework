/* This Program Demonstrates the use or Bitwise Operators
*/  
#include <stdio.h>
#include <string.h>

#define MAX_NIBBLES 64
#define MAX_WORDS MAX_NIBBLES/8

char str[MAX_NIBBLES];
unsigned int binary[MAX_WORDS];

int GetBit(int w, int bit)
{
  return (binary[w] &  (0x00000001 << bit)) ? 1 : 0;
}

/*******************************************************/

void main(void)
{ int i, word, words, nibble, nibbles, bit;
   
  /*** initialize array of binary values ***/
  for (word=0; word<MAX_WORDS; word++) binary[word] = 0;
  for (nibble=0; nibble<MAX_NIBBLES; nibble++) str[nibble] = '\0';
  
  /*** Get a Hex string to store ***/
  printf("Enter a Hex String... ");
  scanf("%s", str);
   
  word = words = nibble = 0;
  nibbles = strlen(str);
  for (i=0; i<nibbles; i++)
  { if ((str[i] >= 'A') && (str[i] <= 'F')) 
    { str[i] -= 55;
    } 
    else if ((str[i] >= 'a') && (str[i] <= 'f'))
    { str[i] -= 87;
    } 
    else if ((str[i] >= '0') && (str[i] <= '9'))
    { str[i] -= 0x30;
    }
    binary[word] |= ((unsigned int)str[i] << ((7-(nibble % 8)) << 2));
    words = word+1;
    if (++nibble >= 8) 
    { nibble = 0; 
      word++;
    }
  }
   
  for (word = 0; word < words; word++)
  { if (str[word]) printf("word[%d] = %0X\n", word, binary[word]);
    for (bit=31; bit>=0; bit--)
    { printf("bit[%2d] = %d\n", bit, GetBit(word, bit));
    } 
    printf("\n");
  }
}
