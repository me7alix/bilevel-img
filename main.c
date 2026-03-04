#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb_image_write.h"

#define PIX(width, ch, x, y) ((y) * (ch) * (width) + (x) * (ch))

#define GRAY_GRID_CNT 5
stbi_uc gray_grid[GRAY_GRID_CNT][2][2] = {
	0,   0,   0,   0,
	255, 0,   0,   0,
	255, 0,   0,   255,
	255, 255, 0,   255,
	255, 255, 255, 255,
};

int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage: INPUT OUTPUT.PNG\n");
		return 1;
	}

	int width, height, channels;
	stbi_uc *img = stbi_load(argv[1], &width, &height, &channels, 0);
	if (!img) {
		fprintf(stderr, "Error in loading image...\n");
		return 1;
	}

	stbi_uc *res = malloc(sizeof(*res) * width * height * channels);

	for (size_t i = 0; i < height; i+=2) {
		for (size_t j = 0; j < width; j+=2) {
			float brightness = 0;

			for (size_t y = 0; y < 2; y++) {
				float res = 0;

				for (size_t x = 0; x < 2; x++) {
					int ind = PIX(width, channels, x + j, y + i);

					for (int i = 0; i < channels; i++) {
						res += img[ind + i];
					}
				}

				brightness += res;
			}

			brightness /= 4 * channels;

			size_t ci = (size_t)(brightness / 255.0 * GRAY_GRID_CNT) - 1;

			for (size_t y = 0; y < 2; y++) {
				for (size_t x = 0; x < 2; x++) {
					res[PIX(width, channels, j+x, i+y) + 0] = gray_grid[ci][y][x];
					res[PIX(width, channels, j+x, i+y) + 1] = gray_grid[ci][y][x];
					res[PIX(width, channels, j+x, i+y) + 2] = gray_grid[ci][y][x];
				}
			}
		}
	}

	int wr = stbi_write_png(argv[2], width, height, channels, res, 0);
	if (wr != 0) {
		printf("Successful...\n");
	} else {
		fprintf(stderr, "Error in image writing...\n");
		return 1;
	}

	stbi_image_free(img);
	free(res);
	return 0;
}
