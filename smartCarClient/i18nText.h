// Copyright [2014] <cuizhaohui>
/**
 * \file i18nText.h
 * \author CUI Zhao-hui, zhh.cui@gmail.com
 * \date 2014-07-01
 * \brief Define a class for puting i18n characters on an image. "FreeType library" is used here.
 */

#ifndef __ZHHCUI_I18NTEXT__
#define __ZHHCUI_I18NTEXT__

#include <opencv2/opencv.hpp>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <cwchar>

struct SizeDesc
{
    float pixelSize;  /// Size of character.
    float space;   /// Default width of character. Only used when FT_Render_Glyph() failed to get character width.
    float gap;  /// Gap between characters.
};

class i18nText {
  public:
    i18nText();
    ~i18nText();
    bool isValid(void);
    bool setFont(const char *name);
    void setSize(float size);
    int putText(cv::Mat &img, const wchar_t *text, cv::Point pos, cv::Scalar color = CV_RGB(0, 0, 0));
  private:
    void putWChar(cv::Mat &img, wchar_t wc, cv::Point &pos, cv::Scalar color);

    static int counter;
    static FT_Library library;
    bool valid;
    FT_Face face;
    SizeDesc size;
};
#endif
