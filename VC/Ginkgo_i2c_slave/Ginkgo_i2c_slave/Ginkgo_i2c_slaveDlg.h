
// Ginkgo_i2c_slaveDlg.h : header file
//

#pragma once


// CGinkgo_i2c_slaveDlg dialog
class CGinkgo_i2c_slaveDlg : public CDialogEx
{
// Construction
public:
	CGinkgo_i2c_slaveDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GINKGO_I2C_SLAVE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
