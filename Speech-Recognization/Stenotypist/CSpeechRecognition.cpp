#include "stdafx.h"
#include "CSpeechRecognition.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpeechRecognition::CSpeechRecognition()
{
}

CSpeechRecognition::~CSpeechRecognition()
{
}

//��ʼ������Initialize�趨������ʶ������Ļ��������������������桢ʶ�������ġ��﷨����Ƶ���¼��ȵĳ�ʼ��
BOOL CSpeechRecognition::Initialize(HWND hWnd,int nIDDlgItem, BOOL bIsShared)
{
    // ��ʼ��com��
    if (FAILED(CoInitialize(NULL))) 
    {
        m_sError=_T("Error intialization COM");
        return FALSE;
    }
    // ��ʼ��ʶ������
    HRESULT hr = S_OK;
    if (bIsShared)
    {
        // �����Ƿ�Ϊ��������ʶ��
        hr = m_cpRecoEngine.CoCreateInstance( CLSID_SpSharedRecognizer );
    }
    else
    {
        hr = m_cpRecoEngine.CoCreateInstance(CLSID_SpInprocRecognizer);
    }
    // RecoContext�ӿ�
    if( SUCCEEDED( hr ) )
    {
        hr = m_cpRecoEngine->CreateRecoContext( &m_cpRecoCtxt );
    }
    // ������Ϣ֪ͨ
    if (SUCCEEDED(hr))
    {
        hr = m_cpRecoCtxt->SetNotifyWindowMessage( hWnd, WM_SREVENT, 0, 0 );
    }
    if (SUCCEEDED(hr))
    {
        // ���ø���Ȥ����Ϣ
        hr = m_cpRecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));
    }
    // ��������ΪĬ������
    CComPtr<ISpAudio> cpAudio;
    hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &cpAudio);
    // ��������
    hr = m_cpRecoEngine->SetInput(cpAudio, TRUE);
    hr = m_cpRecoEngine->SetRecoState( SPRST_ACTIVE );
    // grammar
    if (SUCCEEDED(hr))
    {
        // ������дʱ���õ��﷨
        // ��ʼ���﷨�ӿ�
        hr = m_cpRecoCtxt->CreateGrammar( 0, &m_cpDictationGrammar );
    }
    if  (SUCCEEDED(hr))
    {
        hr = m_cpDictationGrammar->LoadDictation(NULL, SPLO_STATIC);
    }
    if (SUCCEEDED(hr))
    {
        hr = m_cpDictationGrammar->SetDictationState( SPRS_ACTIVE );
    }
    if (FAILED(hr))
    {
        m_cpDictationGrammar.Release();
    }
	 InitTokenList(hWnd,nIDDlgItem);
    return (hr == S_OK);
}
/*/
BOOL CSpeechRecognition::Initialize(HWND hWnd, BOOL bIsShared)
{
	// com library
	if (FAILED(CoInitialize(NULL))) 
	{
		m_sError=_T("Error intialization COM");
		return FALSE;
	}

	// SR engine
	HRESULT hr = S_OK;
	if (bIsShared)
	{
		// Shared reco engine.
		// For a shared reco engine, the audio gets setup automatically
		hr = m_cpRecoEngine.CoCreateInstance( CLSID_SpSharedRecognizer );
	}
	else
	{
		hr = m_cpRecoEngine.CoCreateInstance(CLSID_SpInprocRecognizer);
	}

	// RecoContext
	if( SUCCEEDED( hr ) )
	{
		hr = m_cpRecoEngine->CreateRecoContext( &m_cpRecoCtxt );
	}

	// Set recognition notification for dictation
	if (SUCCEEDED(hr))
	{
		hr = m_cpRecoCtxt->SetNotifyWindowMessage( hWnd, WM_SREVENT, 0, 0 );
	}

	if (SUCCEEDED(hr))
	{
		// when the engine has recognized something
		const ULONGLONG ullInterest = SPFEI(SPEI_RECOGNITION);
		hr = m_cpRecoCtxt->SetInterest(ullInterest, ullInterest);
	}

	// create default audio object
	CComPtr<ISpAudio> cpAudio;
	hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &cpAudio);

	// set the input for the engine
	hr = m_cpRecoEngine->SetInput(cpAudio, TRUE);
	hr = m_cpRecoEngine->SetRecoState( SPRST_ACTIVE );

	// grammar
	if (SUCCEEDED(hr))
	{
		// Specifies that the grammar we want is a dictation grammar.
		// Initializes the grammar (m_cpDictationGrammar)
		hr = m_cpRecoCtxt->CreateGrammar( 0, &m_cpDictationGrammar );
	}
	if  (SUCCEEDED(hr))
	{
		hr = m_cpDictationGrammar->LoadDictation(NULL, SPLO_STATIC);
	}
	if (SUCCEEDED(hr))
	{
		hr = m_cpDictationGrammar->SetDictationState( SPRS_ACTIVE );
	}
	if (FAILED(hr))
	{
		m_cpDictationGrammar.Release();
	}

	return (hr == S_OK);
}
//*/
void CSpeechRecognition::Destroy()
{
	if (m_cpDictationGrammar) 
		m_cpDictationGrammar.Release();
	if (m_cpRecoCtxt) 
		m_cpRecoCtxt.Release();
	if (m_cpRecoEngine) 
		m_cpRecoEngine.Release();
	CoUninitialize();
}
//
BOOL CSpeechRecognition::Start()
{
	if (m_bOnDictation)
	return TRUE;

	HRESULT hr = m_cpRecoEngine->SetRecoState( SPRST_ACTIVE );
	if (FAILED(hr))
	return FALSE;

	m_bOnDictation = TRUE;
	return TRUE;
}
//
BOOL CSpeechRecognition::Stop()
{
	if (! m_bOnDictation)
	return TRUE;

	HRESULT hr = m_cpRecoEngine->SetRecoState( SPRST_INACTIVE );
	if (FAILED(hr))
	return FALSE;

	m_bOnDictation = FALSE;
	return TRUE;
}
//
void CSpeechRecognition::GetText(WCHAR **ppszCoMemText, ULONG ulStart, ULONG nlCount)
{
	USES_CONVERSION;
	CSpEvent event;

	// Process all of the recognition events
	while (event.GetFrom(m_cpRecoCtxt) == S_OK)
	{
		switch (event.eEventId)
		{
			case SPEI_RECOGNITION:
			// There may be multiple recognition results, so get all of them
			{
				HRESULT hr = S_OK;
				if (nlCount == -1)
					event.RecoResult()->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, ppszCoMemText, NULL);
				else
				{
					ASSERT(nlCount > 0);
					event.RecoResult()->GetText(ulStart, nlCount, FALSE, ppszCoMemText, NULL);
				}
			}
			break;
		}
	}
}
/*/
HRESULT CSpeechRecognition::InitTokenList(HWND hWnd, BOOL bIsComboBox)
{
		if (bIsComboBox)
			return SpInitTokenComboBox(hWnd, SPCAT_RECOGNIZERS);
		else
			return SpInitTokenListBox(hWnd, SPCAT_RECOGNIZERS);
}
//*/
HRESULT CSpeechRecognition::InitTokenList(HWND hWnd,int nIDDlgItem )
{
    
        return SpInitTokenComboBox(GetDlgItem(hWnd,nIDDlgItem), SPCAT_RECOGNIZERS);
   
}
//
HRESULT CSpeechRecognition::MicrophoneSetup(HWND hWndParent)
{
	return m_cpRecoEngine->DisplayUI(hWndParent, NULL, SPDUI_MicTraining, NULL, 0);
}
//
HRESULT CSpeechRecognition::VoiceTraining(HWND hWndParent)
{
	return m_cpRecoEngine->DisplayUI(hWndParent, NULL, SPDUI_UserTraining, NULL, 0);
}
//
HRESULT CSpeechRecognition::SetVoice(HWND hWnd,int nIDDlgItem)
{
    HRESULT hr = S_OK;
	CComPtr <ISpVoice> g_cpVoice;
	hr = m_cpRecoCtxt->GetVoice(&g_cpVoice);
	g_cpVoice->SetVoice(SpGetCurSelComboBoxToken(GetDlgItem(hWnd,nIDDlgItem)));
	hr = m_cpRecoCtxt->SetVoice(g_cpVoice,TRUE);
    if(FAILED(hr))
    {
        m_sError = _T("Error to set voice");
        return hr;
    }
    return hr;
}