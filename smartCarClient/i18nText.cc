#include "i18nText.h"

int i18nText::counter = 0;
FT_Library i18nText::library;

i18nText::i18nText() {
    size.pixelSize = 25;
    size.space = 0.5;
    size.gap = 0.1;
    ++counter;
    valid = false;
}

i18nText::~i18nText() {
    if (valid) {
        FT_Done_Face(face);
        valid = false;
    }
    --counter;
    if (!counter) {
        FT_Done_FreeType(library);
    }
}

bool i18nText::isValid(void) {
    return valid;
}

bool i18nText::setFont(const char *name) {
    if (1 == counter) {
        if (FT_Init_FreeType(&library)) {
            return false;
        }
    }
    if (name) {
        if (FT_New_Face(library, name, 0, &face)) {
            return false;
        }
    }
    FT_Set_Pixel_Sizes(face, (int)this->size.pixelSize, 0);
    valid = true;
    return true;
}

void i18nText::setSize(float size) {
    if (size) {
        this->size.pixelSize = fabs(size);
//        this->size.space = fabs(size->space);
//        this->size.gap = fabs(size->gap);
        FT_Set_Pixel_Sizes(face, (int)this->size.pixelSize, 0);
    }
}

int i18nText::putText(cv::Mat &img, const wchar_t *text, cv::Point pos, cv::Scalar color) {
    if (img.rows <= 0 || img.cols <= 0 || !img.data)
        return -1;
    if (text == NULL || text[0] == L'\0')
        return 0;

    wint_t i;
    for (i = 0; text[i] != L'\0'; ++i) {
        putWChar(img, text[i], pos, color);
    }

    return (int)i;
}

void i18nText::putWChar(cv::Mat &img, wchar_t wc, cv::Point &pos, cv::Scalar color) {
    FT_UInt glyph_index = FT_Get_Char_Index(face, wc);
    FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
    FT_GlyphSlot slot = face->glyph;

    int rows = slot->bitmap.rows;
    int cols = slot->bitmap.width;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int off  = ((0 == 0) ? i : (rows - 1 - i)) * slot->bitmap.pitch + j / 8;
            if (slot->bitmap.buffer[off] & (0xC0 >> (j % 8))) {
                int r = (0 == 0) ? pos.y - (rows - 1 - i) : pos.y + i;
                ;
                int c = pos.x + j;

                if (r >= 0 && r < img.rows && c >= 0 && c < img.cols) {
                    cv::Vec3b scalar = img.at<cv::Vec3b>(r, c);

                    for (int k = 0; k < 3; ++k) {
                        scalar.val[k] = color.val[k];
                    }

                    img.at<cv::Vec3b>(r, c) = scalar;
                }
            }
        }
    }

    double space = size.pixelSize * size.space;
    double gap = size.pixelSize * size.gap;

    pos.x += (int)((cols ? cols : space) + gap);
}
