// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FXFA_INCLUDE_XFA_FONTMGR_H_
#define XFA_FXFA_INCLUDE_XFA_FONTMGR_H_

#include <map>
#include <memory>

#include "core/fxcrt/include/fx_ext.h"
#include "core/fxcrt/include/fx_system.h"
#include "xfa/fgas/font/fgas_font.h"
#include "xfa/fxfa/include/fxfa.h"

class CPDF_Font;

struct XFA_FONTINFO {
  uint32_t dwFontNameHash;
  const FX_WCHAR* pPsName;
  const FX_WCHAR* pReplaceFont;
  uint16_t dwStyles;
  uint16_t wCodePage;
};

class CXFA_DefFontMgr {
 public:
  CXFA_DefFontMgr() {}
  ~CXFA_DefFontMgr();

  IFGAS_Font* GetFont(CXFA_FFDoc* hDoc,
                      const CFX_WideStringC& wsFontFamily,
                      uint32_t dwFontStyles,
                      uint16_t wCodePage = 0xFFFF);
  IFGAS_Font* GetDefaultFont(CXFA_FFDoc* hDoc,
                             const CFX_WideStringC& wsFontFamily,
                             uint32_t dwFontStyles,
                             uint16_t wCodePage = 0xFFFF);

 protected:
  CFX_ArrayTemplate<IFGAS_Font*> m_CacheFonts;
};

class CXFA_PDFFontMgr {
 public:
  explicit CXFA_PDFFontMgr(CXFA_FFDoc* pDoc);
  ~CXFA_PDFFontMgr();

  IFGAS_Font* GetFont(const CFX_WideStringC& wsFontFamily,
                      uint32_t dwFontStyles,
                      CPDF_Font** pPDFFont,
                      FX_BOOL bStrictMatch = TRUE);
  FX_BOOL GetCharWidth(IFGAS_Font* pFont,
                       FX_WCHAR wUnicode,
                       int32_t& iWidth,
                       FX_BOOL bCharCode);
  std::map<IFGAS_Font*, CPDF_Font*> m_FDE2PDFFont;

 protected:
  IFGAS_Font* FindFont(CFX_ByteString strFamilyName,
                       FX_BOOL bBold,
                       FX_BOOL bItalic,
                       CPDF_Font** pPDFFont,
                       FX_BOOL bStrictMatch = TRUE);
  CFX_ByteString PsNameToFontName(const CFX_ByteString& strPsName,
                                  FX_BOOL bBold,
                                  FX_BOOL bItalic);
  FX_BOOL PsNameMatchDRFontName(const CFX_ByteStringC& bsPsName,
                                FX_BOOL bBold,
                                FX_BOOL bItalic,
                                const CFX_ByteString& bsDRFontName,
                                FX_BOOL bStrictMatch = TRUE);

  CXFA_FFDoc* m_pDoc;
  std::map<CFX_ByteString, IFGAS_Font*> m_FontMap;
};

class CXFA_FontMgr {
 public:
  CXFA_FontMgr();
  ~CXFA_FontMgr();

  IFGAS_Font* GetFont(CXFA_FFDoc* hDoc,
                      const CFX_WideStringC& wsFontFamily,
                      uint32_t dwFontStyles,
                      uint16_t wCodePage = 0xFFFF);
  void LoadDocFonts(CXFA_FFDoc* hDoc);
  void ReleaseDocFonts(CXFA_FFDoc* hDoc);
  void SetDefFontMgr(std::unique_ptr<CXFA_DefFontMgr> pFontMgr);

 protected:
  std::unique_ptr<CXFA_DefFontMgr> m_pDefFontMgr;
  std::map<CXFA_FFDoc*, std::unique_ptr<CXFA_PDFFontMgr>> m_PDFFontMgrMap;
  std::map<CFX_ByteString, IFGAS_Font*> m_FontMap;
};

#endif  //  XFA_FXFA_INCLUDE_XFA_FONTMGR_H_
