// BMPextractRGBtoHex.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
typedef struct {
	unsigned char signature[2];
	unsigned int fileSize;
	unsigned int reserved;
	unsigned int dataOffset;
} BMPHeader;

typedef struct {
	unsigned int headerSize;
	int width;
	int height;
	unsigned short planes;
	unsigned short bitsPerPixel;
	unsigned int compression;
	unsigned int imageSize;
	int xPixelsPerMeter;
	int yPixelsPerMeter;
	unsigned int totalColors;
	unsigned int importantColors;
} BMPInfoHeader;
#pragma pack(pop)

void extractRGB(const char* filename, const char* outputFilename) {
	FILE* file = fopen(filename, "rb");
	if (file == NULL) {
		printf("Failed to open the file.\n");
		return;
	}

	BMPHeader header;
	fread(&header, sizeof(BMPHeader), 1, file);

	BMPInfoHeader infoHeader;
	fread(&infoHeader, sizeof(BMPInfoHeader), 1, file);

	int width = infoHeader.width;
	int height = infoHeader.height;
	printf("width = %d\n", width);
	printf("height = %d\n", height);

	int padding = (4 - (width * 3) % 4) % 4;
	int rowSize = (width * 3) + padding;
	printf("padding = %d\n", padding);
	printf("rowSize = %d\n", rowSize);

	fseek(file, header.dataOffset, SEEK_SET);

	unsigned char* imageData = (unsigned char*)malloc(rowSize * height);
	fread(imageData, rowSize * height, 1, file);

	fclose(file);

	FILE* outputFile = fopen(outputFilename, "w");
	if (outputFile == NULL) {
		printf("Failed to create the output file.\n");
		free(imageData);
		return;
	}

	// 提取RGB像素值并输出为HEX文件
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = (y * rowSize) + (x * 3);
			unsigned char blue = imageData[index + 0];
			unsigned char green = imageData[index + 1];
			unsigned char red = imageData[index + 2];

			// 将RGB像素值转换为HEX格式并输出到文件
			if (x == width - 1)
				fprintf(outputFile, "#%02X%02X%02X\n", red, green, blue);
			else
				fprintf(outputFile, "#%02X%02X%02X, ", red, green, blue);
		}
	}

	fclose(outputFile);
	free(imageData);
}

int main() {
	const char* filename = "B_01.bmp";
	const char* outputFilename = "output.hex";
	extractRGB(filename, outputFilename);

	return 0;
}
