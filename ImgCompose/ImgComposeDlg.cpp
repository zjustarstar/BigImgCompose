
// ImgComposeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgCompose.h"
#include "ImgComposeDlg.h"
#include "afxdialogex.h"
#include <fstream>

#include <opencv2/highgui/highgui_c.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG  
#pragma comment(lib,"opencv_world400d.lib")
#else  
#pragma comment(lib,"opencv_world400.lib")
#endif

UINT ThreadComposeImg(LPVOID pParam) {
	CImgComposeDlg * pDlg = (CImgComposeDlg *)pParam;

	for (int i = 0; i < IMGSET_NUM; i++)
	{
		char strFileName[256];
		sprintf_s(strFileName, "d:\\compose_%d.jpg", i + 1);

		pDlg->m_strProgInfo.Format("正在合成第%d张图像", i + 1);
		pDlg->ComposeImg(pDlg->m_vecImgSet[i], string(strFileName));
	}

	ofstream of1;
	of1.open("d:\\compose_record.txt", ios::out);//打开文件
	for (int i = 0; i <pDlg->m_vecSecSubDir.size(); i++)
	{
		of1 << i << ":" << pDlg->m_vecSecSubDir[i] << "\n";
	}
	of1.close();

	pDlg->m_strProgInfo = "处理完成！";

	return 1;
}

// CImgComposeDlg 对话框

CImgComposeDlg::CImgComposeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMGCOMPOSE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImgComposeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FOLDERPATH, m_editFilePath);
	DDX_Control(pDX, IDC_STATIC_PROGINFO, m_sttProgInfo);
	DDX_Control(pDX, IDC_STATIC_FOLDERINFO, m_sttFolderInfo);
}

BEGIN_MESSAGE_MAP(CImgComposeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CImgComposeDlg::OnBnClickedButtonProcess)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CImgComposeDlg::OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// CImgComposeDlg 消息处理程序

BOOL CImgComposeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	m_strFilePath = "";
	m_bValid = TRUE;
	m_sttProgInfo.SetWindowTextA("");
	//
	SetTimer(1000, 1000, NULL);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CImgComposeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CImgComposeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CImgComposeDlg::OnBnClickedButtonProcess()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//罗列文件夹信息;
	if (m_strFilePath.IsEmpty()){
		AfxMessageBox("请先选择处理路径");
		return;
	}
	if (!m_bValid){
		AfxMessageBox("存在不合规的文件");
		return;
	}

	AfxBeginThread(ThreadComposeImg, (void *)this, THREAD_PRIORITY_HIGHEST);

	//进行合成;
	/*ComposeImg(m_vecImgSet[0], "d:\\comp1.jpg");
	ComposeImg(m_vecImgSet[1], "d:\\comp2.jpg");
	ComposeImg(m_vecImgSet[2], "d:\\comp3.jpg");
	ComposeImg(m_vecImgSet[3], "d:\\comp4.jpg");
	ComposeImg(m_vecImgSet[4], "d:\\comp5.jpg");
	ComposeImg(m_vecImgSet[5], "d:\\comp6.jpg");
	*/

}

//根据输入的图像集,生成合成图;
void CImgComposeDlg::ComposeImg(vector<string> vecImgSet,string strFilePath) {
	Mat compImg;
	int nCompW = 13278;
	int nCompH = 3317;
	int nSingleImgSize = 657;
	compImg = cv::Mat(nCompH, nCompW, CV_8UC3, Scalar(255, 255, 255));

	//7-8-7-7-7-8-7-7-7 -8- 7-7-7-8-7-7-7-8-7
	//总和=98+40=138
	int xSpace[19] = {7,8,7,7,7,8,7,7,7,8,7,7,7,8,7,7,7,8,7};
	int ySpace = 8;

	//从右下角开始合成;
	Rect dstRect;
	dstRect.width = nSingleImgSize;
	dstRect.height = nSingleImgSize;
	dstRect.x = 0;
	dstRect.y = 0;

	int nTotalDeltaX=0;
	for (int i = 0; i < vecImgSet.size(); i++)
	{
		Mat curImg;
		Mat resizedImg;
		curImg = imread(vecImgSet[i]);
		resize(curImg, resizedImg, cvSize(nSingleImgSize, nSingleImgSize));

		//从右下角开始合成;
		int nDeltaX, nDeltaY;
		//最右边的
		if (i % 20 == 0)
		{
			nDeltaX = nSingleImgSize;

			//第二行开始重新计数;
			nTotalDeltaX = 0;
		}
		else
		{
			nDeltaX = nSingleImgSize + xSpace[i % 20];
		}

		nTotalDeltaX += nDeltaX;
		dstRect.x = nCompW - nTotalDeltaX;
		//最左边的;
		if ((i + 1) % 20 == 0)
			dstRect.x = 0;

		dstRect.y = (nSingleImgSize + ySpace) * (4 - (i / 20));

		resizedImg.copyTo(compImg(dstRect));
	}

	vector<int> vecCompParam;
	vecCompParam.push_back(1); //CV_IMWRITE_JPEG_QUALITY
	vecCompParam.push_back(99);
	imwrite(strFilePath, compImg);
}

int CImgComposeDlg::GetAllSubDir(CString strPath,vector<string>& vecDir) {
	CFileFind findFile;
	CString   strDstFiles;

	strDstFiles = strPath + "\\*.*";
	BOOL bWorking = findFile.FindFile(strDstFiles);

	while (bWorking)
	{
		bWorking = findFile.FindNextFile();
		if (findFile.IsDots())
			continue;
		if (findFile.IsDirectory())//是文件夹
		{
			CString strSubDir = findFile.GetFileName();//文件夹名称
			strSubDir = strPath + "\\" + strSubDir;

			string strTest;
			strTest = CA2A(strSubDir.GetString());
			vecDir.push_back(strTest);
		}
	}
	findFile.Close();

	return vecDir.size();
}


//每个子文件夹只需要有6张图即可，不限格式;
//其中vecMore和vecLess保存了超过6张或者不足6张的目录信息
int CImgComposeDlg::GetAllImgFiles_Any(string strFilePath, vector<string>& vecMoreFile, vector<string>& vecLessFile)
{
	//获得当前文件夹下的所有图像;
	CString  strTemp;
	strTemp = CA2A(strFilePath.c_str());

	CFileFind findFile;
	CString   strDstFiles;

	strDstFiles = strTemp + "\\*.jpg";
	BOOL bWorking = findFile.FindFile(strDstFiles);

	int nCount = 0;
	bool bMore = false;
	while (bWorking)
	{
		bWorking = findFile.FindNextFile();
		CString strFileName = findFile.GetFileName();

		string strTemp;
		strTemp = CA2A(strFileName.GetString());
		//加上路径;
		strTemp = strFilePath + "\\" + strTemp;
		if (nCount<IMGSET_NUM)
			m_vecImgSet[nCount].push_back(strTemp);

		nCount++;	
	}
	//如果光jpg图就超了。。。
	if (bMore)
	{
		vecMoreFile.push_back(strFilePath);
		m_bValid = false;
		findFile.Close();
		return nCount + 1;
	}

	strDstFiles = strTemp + "\\*.png";
	bWorking = findFile.FindFile(strDstFiles);
	while (bWorking)
	{
		bWorking = findFile.FindNextFile();
		CString strFileName = findFile.GetFileName();

		string strTemp;
		strTemp = CA2A(strFileName.GetString());
		//加上路径;
		strTemp = strFilePath + "\\" + strTemp;
		if (nCount < IMGSET_NUM)
			m_vecImgSet[nCount].push_back(strTemp);

		nCount++;

	}

	findFile.Close();

	//图像数据过多
	if (nCount>IMGSET_NUM)
	{
		vecMoreFile.push_back(strFilePath);
		m_bValid = false;
	}
	//图像数据过少
	else if (nCount < IMGSET_NUM)
	{
		vecLessFile.push_back(strFilePath);
		m_bValid = false;
	}

	return nCount;
}


//要求每个文件夹下有特殊格式;
int CImgComposeDlg::GetAllImgFiles(string strFilePath)
{
	//获得当前文件夹下的所有图像;
	CString  strTemp;
	strTemp = CA2A(strFilePath.c_str());

	CFileFind findFile;
	CString   strDstFiles;

	strDstFiles = strTemp + "\\*.jpg";
	BOOL bWorking = findFile.FindFile(strDstFiles);

	int nCount = 0;
	int nImgSetCount[6];
	memset(nImgSetCount, 0, sizeof(int) * 6);

	while (bWorking)
	{
		bWorking = findFile.FindNextFile();
		CString strFileName = findFile.GetFileName();

		string strTemp;
		strTemp = CA2A(strFileName.GetString());
		//加上路径;
		strTemp = strFilePath + "\\" + strTemp;
		//未找到'-'，为第一个;
		if (strFileName.Find('-') == -1){
			m_vecImgSet[0].push_back(strTemp);
			nImgSetCount[0]++;
		}
		else {
			int nFinalDight;
			// *-d.jpg
			nFinalDight = strFileName.GetLength() - 5;
			char c = strFileName.GetAt(nFinalDight);
			int n = c - '0';

			//合规性确认;
			if (n<=0 || n>5){
				m_bValid = false;
				continue;
			}
			else {
				m_vecImgSet[n].push_back(strTemp);
				nImgSetCount[n]++;
			}
		}

		nCount++;

	}

	strDstFiles = strTemp + "\\*.png";
	bWorking = findFile.FindFile(strDstFiles);

	while (bWorking)
	{
		bWorking = findFile.FindNextFile();
		CString strFileName = findFile.GetFileName();

		string strTemp;
		strTemp = CA2A(strFileName.GetString());
		//加上路径;
		strTemp = strFilePath + "\\" + strTemp;
		//未找到'-'，为第一个;
		if (strFileName.Find('-') == -1) {
			m_vecImgSet[0].push_back(strTemp);
			nImgSetCount[0]++;
		}
		else {
			int nFinalDight;
			// *-d.jpg
			nFinalDight = strFileName.GetLength() - 5;
			char c = strFileName.GetAt(nFinalDight);
			int n = c - '0';

			//合规性确认;
			if (n <= 0 || n > 5) {
				m_bValid = false;
				continue;
			}
			else {
				m_vecImgSet[n].push_back(strTemp);
				nImgSetCount[n]++;
			}
		}

		nCount++;

	}

	findFile.Close();

	return nCount;
}


void CImgComposeDlg::OnBnClickedButtonBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bi;
	char Buffer[MAX_PATH];

	//初始化入口参数bi开始
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;//初始化制定的root目录很不容易，
	bi.pszDisplayName = Buffer;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = "选择测试文件夹";
	bi.lpfn = NULL;
	bi.ulFlags = BIF_EDITBOX;//包括文件

	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if (pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//取得文件夹路径到Buffer里
		m_strFilePath.Format("%s", Buffer);
		m_editFilePath.SetWindowText(m_strFilePath);
	}

	//清空及初始化;
	m_vecSubDir.clear();
	m_vecSecSubDir.clear();
	for (int i = 0; i < IMGSET_NUM; i++)
		m_vecImgSet[i].clear();
	m_bValid = true;

	//一级子文件夹
	int nSubDirSize = GetAllSubDir(m_strFilePath,m_vecSubDir);

	//二级子文件夹;
	for (int i = 0; i < nSubDirSize; i++){
		CString strTemp;
		strTemp = CA2A(m_vecSubDir[i].c_str());
		GetAllSubDir(strTemp, m_vecSecSubDir);
	}
	int nSecSubDirSize = m_vecSecSubDir.size();

	//图像文件;
	int nImgCount = 0;
	int nProcImgNum = nSecSubDirSize;
	if (nProcImgNum > 100)
		nProcImgNum = 100;
	//仅取前100张图进行计算;
	vector<string> vecMoreFile, vecLessFile;
	for (int i = 0; i < nProcImgNum; i++)
	{
		string strPath = m_vecSecSubDir[i];
		int tempCount = GetAllImgFiles_Any(strPath,vecMoreFile,vecLessFile);
		
		nImgCount += tempCount;
	}

	CString strInfo;
	strInfo.Format("共有%d个一级子文件夹,%d个二级子文件夹(超过100取前100个)，%d个图像文件", nSubDirSize, nSecSubDirSize, nImgCount);
	if (!m_bValid)
	{

		if (vecLessFile.size() > 0)
		{
			strInfo += ",少于6张图文件夹有:";
			for (int i = 0; i < vecLessFile.size(); i++)
			{
				CString strTemp;
				strTemp = CA2A(vecLessFile[i].c_str());
				strTemp = strTemp.Right(strTemp.GetLength() - m_strFilePath.GetLength() - 1);
				strInfo = strInfo + strTemp + ",";
			}
		}

		if (vecMoreFile.size() > 0)
		{
			strInfo += "超过6张图(超过6张取前6张)文件夹有:";
			for (int i = 0; i < vecMoreFile.size(); i++)
			{
				CString strTemp;
				strTemp = CA2A(vecMoreFile[i].c_str());
				strTemp = strTemp.Right(strTemp.GetLength() - m_strFilePath.GetLength() - 1);
				strInfo = strInfo + strTemp + ",";
			}
		}
		
	}

	m_sttFolderInfo.SetWindowTextA(strInfo);
}

void CImgComposeDlg::OnTimer(UINT_PTR nIDEvent)
{
	m_sttProgInfo.SetWindowTextA(m_strProgInfo);
}
