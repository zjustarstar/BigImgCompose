
// ImgComposeDlg.h : 头文件
//

#pragma once
#include <opencv2\opencv.hpp>
#include "afxwin.h"

#define IMGSET_NUM    6

using namespace std;
using namespace cv;

// CImgComposeDlg 对话框
class CImgComposeDlg : public CDialogEx
{
// 构造
public:
	CImgComposeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMGCOMPOSE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString      m_strFilePath;  //文件夹路径;
	CEdit        m_editFilePath;
	CString      m_strProgInfo;  //进度信息;
	vector<string>  m_vecSubDir;     //一级文件夹;
	vector<string>  m_vecSecSubDir;  //二级子文件夹;
	vector<string>  m_vecImgSet[IMGSET_NUM];  //6个图像文件集;
	bool         m_bValid;           //合规性;
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
