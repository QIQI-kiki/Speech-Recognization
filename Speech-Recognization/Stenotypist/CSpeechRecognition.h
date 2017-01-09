///////////////////////////////////////////////////////////////
// active speech engine
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <sapi.h>
#include <sphelper.h>
#include <spuihelp.h>
///////////////////////////////////////////////////////////////
// speech message
#define WM_SREVENT	WM_USER+102

class CSpeechRecognition  
{
	public:
		CSpeechRecognition();
		virtual ~CSpeechRecognition();
		// initialize
		BOOL Initialize(HWND hWnd ,int nIDDlgItem, BOOL bIsShared = TRUE);

		void Destroy();		
		// start and stop
		BOOL Start();
		BOOL Stop();
		
		BOOL IsDictationOn()
		{
			return m_bOnDictation;
		}

		// event handler
		void GetText(WCHAR **ppszCoMemText, ULONG ulStart = 0, ULONG nlCount = -1);

		// voice training
		HRESULT VoiceTraining(HWND hWndParent);

		// microphone setup
		HRESULT MicrophoneSetup(HWND hWndParent);

		//set voice
		HRESULT SetVoice(HWND hWnd,int nIDDlgItem);

		// token list
		HRESULT InitTokenList(HWND hWnd,int nIDDlgItem);

		// error string
		CString GetErrorString()
		{
			return m_sError;
		}

		// interface
		CComPtr <ISpRecognizer>  m_cpRecoEngine;	// SR engine
		CComPtr <ISpRecoContext> m_cpRecoCtxt;	//Recognition contextfor dictation
		CComPtr <ISpRecoGrammar> m_cpDictationGrammar;  // Dictation grammar 

		private:
			CString m_sError;
			BOOL    m_bOnDictation;
};
