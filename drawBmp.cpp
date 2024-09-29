#include <iostream>
#include <fstream>
#include <windows.h>

class BMPImage {
public:
    BMPImage() : file(nullptr), data(nullptr), width(0), height(0) {}

    bool openBMP(const std::string& fileName) {
        file = new std::ifstream(fileName, std::ios::binary);
        if (!file->is_open()) {
            std::cerr << "Ошибка открытия файла: " << fileName << std::endl;
            return false;
        }

        BITMAPFILEHEADER fileHeader;
        file->read((char*)&fileHeader, sizeof(BITMAPFILEHEADER));

        BITMAPINFOHEADER infoHeader;
        file->read((char*)&infoHeader, sizeof(BITMAPINFOHEADER));

        width = infoHeader.biWidth;
        height = abs(infoHeader.biHeight);

        data = new unsigned char[width * height * 3]; 
        file->seekg(fileHeader.bfOffBits);
        file->read((char*)data, width * height * 3);

        return true;
    }

    void displayBMP() {
        if (!data) {
            std::cerr << "Изображение не загружено." << std::endl;
            return;
        }

        for (int y = height - 1; y >= 0; --y) {
            for (int x = 0; x < width; ++x) {
                int index = (y * width + x) * 3;
                unsigned char blue = data[index];
                unsigned char green = data[index + 1];
                unsigned char red = data[index + 2];

                if (red == 0 && green == 0 && blue == 0) {
                    std::cout << "#"; 
                } else {
                    std::cout << " "; 
                }
            }
            std::cout << std::endl;
        }
    }

    void closeBMP() {
        if (file) {
            file->close();
            delete file;
            file = nullptr;
        }
        if (data) {
            delete[] data;
            data = nullptr;
        }
    }

private:
    std::ifstream* file;
    unsigned char* data;
    int width;
    int height;
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Использование: drawBmp.exe <путь_к_файлу.bmp>" << std::endl;
        return 1;
    }

    BMPImage image;
    if (image.openBMP(argv[1])) {
        image.displayBMP();
        image.closeBMP();
    }

    return 0;
}