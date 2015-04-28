//----------------------------------------------------------------------------
#ifndef SDIMainH
#define SDIMainH
//----------------------------------------------------------------------------
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <StdCtrls.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
//----------------------------------------------------------------------------
class TSDIAppForm : public TForm
{
__published:
	TEdit *WriteDataEdit;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *ReadDataEdit;
	TButton *WriteDataButton;
	TButton *ReadDataButton;
	void __fastcall WriteDataButtonClick(TObject *Sender);
	void __fastcall ReadDataButtonClick(TObject *Sender);
private:
public:
    virtual __fastcall TSDIAppForm(TComponent *AOwner);
};
//----------------------------------------------------------------------------
extern TSDIAppForm *SDIAppForm;
//----------------------------------------------------------------------------
#endif
