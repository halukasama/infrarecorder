/*
 * InfraRecorder - CD/DVD burning software
 * Copyright (C) 2006-2012 Christian Kindahl
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <vector>
#include <base/string_util.hh>

class CFilesDataObject : public IDataObject
{
private:
    long m_lRefCount;

    FORMATETC m_FormatEtc;
    STGMEDIUM m_StgMedium;

    std::vector<tstring> m_Files;

    bool IsFormatSupported(FORMATETC *pFormatEtc);

public:
    CFilesDataObject();
    ~CFilesDataObject();

    void Reset();
    void AddFile(const TCHAR *szFileName);

    // IUnknown members.
    HRESULT __stdcall QueryInterface(REFIID iid,void **ppvObject);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();
        
    // IDataObject members.
    HRESULT __stdcall GetData(FORMATETC *pFormatEtc,STGMEDIUM *pmedium);
    HRESULT __stdcall GetDataHere(FORMATETC *pFormatEtc,STGMEDIUM *pmedium);
    HRESULT __stdcall QueryGetData(FORMATETC *pFormatEtc);
    HRESULT __stdcall GetCanonicalFormatEtc(FORMATETC *pFormatEct,FORMATETC *pFormatEtcOut);
    HRESULT __stdcall SetData(FORMATETC *pFormatEtc,STGMEDIUM *pMedium,BOOL fRelease);
    HRESULT __stdcall EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc);
    HRESULT __stdcall DAdvise(FORMATETC *pFormatEtc,DWORD advf,IAdviseSink *,DWORD *);
    HRESULT __stdcall DUnadvise(DWORD dwConnection);
    HRESULT __stdcall EnumDAdvise(IEnumSTATDATA **ppEnumAdvise);
};
