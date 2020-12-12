
// ImgComposeDlg.cpp : ʵ���ļ�
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

		pDlg->m_strProgInfo.Format("���ںϳɵ�%d��ͼ��", i + 1);
		pDlg->ComposeImg(pDlg->m_vecImgSet[i], string(strFileName));
	}

	ofstream of1;
	of1.open("d:\\compose_record.txt", ios::out);//���ļ�
	for (int i = 0; i <pDlg->m_vecSecSubDir.size(); i++)
	{
		of1 << i << ":" << pDlg->m_vecSecSubDir[i] << "\n";
	}
	of1.close();

	pDlg->m_strProgInfo = "������ɣ�";

	return 1;
}

// CImgComposeDlg �Ի���

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


// CImgComposeDlg ��Ϣ�������

BOOL CImgComposeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	m_strFilePath = "";
	m_bValid = TRUE;
	m_sttProgInfo.SetWindowTextA("");
	//
	SetTimer(1000, 1000, NULL);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CImgComposeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CImgComposeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CImgComposeDlg::OnBnClickedButtonProcess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//�����ļ�����Ϣ;
	if (m_strFilePath.IsEmpty()){
		AfxMessageBox("����ѡ����·��");
		return;
	}
	if (!m_bValid){
		AfxMessageBox("���ڲ��Ϲ���ļ�");
		return;
	}

	AfxBeginThread(ThreadComposeImg, (void *)this, THREAD_PRIORITY_HIGHEST);

	//���кϳ�;
	/*ComposeImg(m_vecImgSet[0], "d:\\comp1.jpg");
	ComposeImg(m_vecImgSet[1], "d:\\comp2.jpg");
	ComposeImg(m_vecImgSet[2], "d:\\comp3.jpg");
	ComposeImg(m_vecImgSet[3], "d:\\comp4.jpg");
	ComposeImg(m_vecImgSet[4], "d:\\comp5.jpg");
	ComposeImg(m_vecImgSet[5], "d:\\comp6.jpg");
	*/

}

//���������ͼ��,���ɺϳ�ͼ;
void CImgComposeDlg::ComposeImg(vector<string> vecImgSet,string strFilePath) {
	Mat compImg;
	int nCompW = 13278;
	int nCompH = 3317;
	int nSingleImgSize = 657;
	compImg = cv::Mat(nCompH, nCompW, CV_8UC3, Scalar(255, 255, 255));

	//7-8-7-7-7-8-7-7-7 -8- 7-7-7-8-7-7-7-8-7
	//�ܺ�=98+40=138
	int xSpace[19] = {7,8,7,7,7,8,7,7,7,8,7,7,7,8,7,7,7,8,7};
	int ySpace = 8;

	//�����½ǿ�ʼ�ϳ�;
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

		//�����½ǿ�ʼ�ϳ�;
		int nDeltaX, nDeltaY;
		//���ұߵ�
		if (i % 20 == 0)
		{
			nDeltaX = nSingleImgSize;

			//�ڶ��п�ʼ���¼���;
			nTotalDeltaX = 0;
		}
		else
		{
			nDeltaX = nSingleImgSize + xSpace[i % 20];
		}

		nTotalDeltaX += nDeltaX;
		dstRect.x = nCompW - nTotalDeltaX;
		//����ߵ�;
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
		if (findFile.IsDirectory())//���ļ���
		{
			CString strSubDir = findFile.GetFileName();//�ļ�������
			strSubDir = strPath + "\\" + strSubDir;

			string strTest;
			strTest = CA2A(strSubDir.GetString());
			vecDir.push_back(strTest);
		}
	}
	findFile.Close();

	return vecDir.size();
}


//ÿ�����ļ���ֻ��Ҫ��6��ͼ���ɣ����޸�ʽ;
//����vecMore��vecLess�����˳���6�Ż��߲���6�ŵ�Ŀ¼��Ϣ
int CImgComposeDlg::GetAllImgFiles_Any(string strFilePath, vector<string>& vecMoreFile, vector<string>& vecLessFile)
{
	//��õ�ǰ�ļ����µ�����ͼ��;
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
		//����·��;
		strTemp = strFilePath + "\\" + strTemp;
		if (nCount<IMGSET_NUM)
			m_vecImgSet[nCount].push_back(strTemp);

		nCount++;	
	}
	//�����jpgͼ�ͳ��ˡ�����
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
		//����·��;
		strTemp = strFilePath + "\\" + strTemp;
		if (nCount < IMGSET_NUM)
			m_vecImgSet[nCount].push_back(strTemp);

		nCount++;

	}

	findFile.Close();

	//ͼ�����ݹ���
	if (nCount>IMGSET_NUM)
	{
		vecMoreFile.push_back(strFilePath);
		m_bValid = false;
	}
	//ͼ�����ݹ���
	else if (nCount < IMGSET_NUM)
	{
		vecLessFile.push_back(strFilePath);
		m_bValid = false;
	}

	return nCount;
}


//Ҫ��ÿ���ļ������������ʽ;
int CImgComposeDlg::GetAllImgFiles(string strFilePath)
{
	//��õ�ǰ�ļ����µ�����ͼ��;
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
		//����·��;
		strTemp = strFilePath + "\\" + strTemp;
		//δ�ҵ�'-'��Ϊ��һ��;
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

			//�Ϲ���ȷ��;
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
		//����·��;
		strTemp = strFilePath + "\\" + strTemp;
		//δ�ҵ�'-'��Ϊ��һ��;
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

			//�Ϲ���ȷ��;
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO bi;
	char Buffer[MAX_PATH];

	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;//��ʼ���ƶ���rootĿ¼�ܲ����ף�
	bi.pszDisplayName = Buffer;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = "ѡ������ļ���";
	bi.lpfn = NULL;
	bi.ulFlags = BIF_EDITBOX;//�����ļ�

	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if (pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//ȡ���ļ���·����Buffer��
		m_strFilePath.Format("%s", Buffer);
		m_editFilePath.SetWindowText(m_strFilePath);
	}

	//��ռ���ʼ��;
	m_vecSubDir.clear();
	m_vecSecSubDir.clear();
	for (int i = 0; i < IMGSET_NUM; i++)
		m_vecImgSet[i].clear();
	m_bValid = true;

	//һ�����ļ���
	int nSubDirSize = GetAllSubDir(m_strFilePath,m_vecSubDir);

	//�������ļ���;
	for (int i = 0; i < nSubDirSize; i++){
		CString strTemp;
		strTemp = CA2A(m_vecSubDir[i].c_str());
		GetAllSubDir(strTemp, m_vecSecSubDir);
	}
	int nSecSubDirSize = m_vecSecSubDir.size();

	//ͼ���ļ�;
	int nImgCount = 0;
	int nProcImgNum = nSecSubDirSize;
	if (nProcImgNum > 100)
		nProcImgNum = 100;
	//��ȡǰ100��ͼ���м���;
	vector<string> vecMoreFile, vecLessFile;
	for (int i = 0; i < nProcImgNum; i++)
	{
		string strPath = m_vecSecSubDir[i];
		int tempCount = GetAllImgFiles_Any(strPath,vecMoreFile,vecLessFile);
		
		nImgCount += tempCount;
	}

	CString strInfo;
	strInfo.Format("����%d��һ�����ļ���,%d���������ļ���(����100ȡǰ100��)��%d��ͼ���ļ�", nSubDirSize, nSecSubDirSize, nImgCount);
	if (!m_bValid)
	{

		if (vecLessFile.size() > 0)
		{
			strInfo += ",����6��ͼ�ļ�����:";
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
			strInfo += "����6��ͼ(����6��ȡǰ6��)�ļ�����:";
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
