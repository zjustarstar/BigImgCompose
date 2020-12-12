
// ImgComposeDlg.h : ͷ�ļ�
//

#pragma once
#include <opencv2\opencv.hpp>
#include "afxwin.h"

#define IMGSET_NUM    6

using namespace std;
using namespace cv;

// CImgComposeDlg �Ի���
class CImgComposeDlg : public CDialogEx
{
// ����
public:
	CImgComposeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMGCOMPOSE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString      m_strFilePath;  //�ļ���·��;
	CEdit        m_editFilePath;
	CString      m_strProgInfo;  //������Ϣ;
	vector<string>  m_vecSubDir;     //һ���ļ���;
	vector<string>  m_vecSecSubDir;  //�������ļ���;
	vector<string>  m_vecImgSet[IMGSET_NUM];  //6��ͼ���ļ���;
	bool         m_bValid;           //�Ϲ���;
	void ComposeImg(vector<string> vecImgSet, string strFilePath);
	int GetAllSubDir(CString strPath, vector<string>& vecDir);
	int GetAllImgFiles(string strFilePath);
	int GetAllImgFiles_Any(string strFilePath, vector<string>& vecMoreFile, vector<string>& vecLessFile);
	afx_msg void OnBnClickedButtonProcess();
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_sttProgInfo;
	CStatic m_sttFolderInfo;
};
