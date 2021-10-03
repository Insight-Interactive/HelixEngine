#pragma once


class FileExplorerWindow
{
public:
	struct Description
	{
		WChar* Title;
		bool FilterFoldersOnly;
		bool ShowImmediate;
	};

public:
	FileExplorerWindow();
	~FileExplorerWindow();
	
	bool IsValid();

	bool Create( const Description& Desc );
	void Show( WChar** Result );
	void Close();

private:

#if HE_WINDOWS_DESKTOP
	IFileDialog* m_pFileDialog;

#endif
};
