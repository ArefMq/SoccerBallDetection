#include "singleimageloader.h"

SingleImageLoader::SingleImageLoader() : StreamLoader()
{

}

void SingleImageLoader::open(const char* path)
{
    _frame = QImage(path);
}

