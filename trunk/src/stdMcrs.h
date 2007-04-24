/*************************************************

	SIGMA DABSTER 5
	(c) 2007 Zespó³ Dabstera

	Standard Macros Header
	stdMcrs.h

*************************************************/

#ifndef _STD_MCRS_H_
#define _STD_MCRS_H_

//kolory

#define _white			System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), \
														 static_cast<System::Int32>(static_cast<System::Byte>(255)), \
														 static_cast<System::Int32>(static_cast<System::Byte>(255)))
#define _black			System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), \
														 static_cast<System::Int32>(static_cast<System::Byte>(0)), \
														 static_cast<System::Int32>(static_cast<System::Byte>(0)))
#define _lightBrown		System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(236)), \
														 static_cast<System::Int32>(static_cast<System::Byte>(233)), \
														 static_cast<System::Int32>(static_cast<System::Byte>(216)))
#define _lightBlue		System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(187)), \
														 static_cast<System::Int32>(static_cast<System::Byte>(211)), \
														 static_cast<System::Int32>(static_cast<System::Byte>(249)))
#define _darkBlue		System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(49)), \
														 static_cast<System::Int32>(static_cast<System::Byte>(106)), \
														 static_cast<System::Int32>(static_cast<System::Byte>(197)))
#define _transparent	System::Drawing::Color::Transparent

//czcionki

#define _sansSerif825		gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238))
#define _sansSerif825Bold	gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238))
#define _arial9				gcnew System::Drawing::Font(L"Arial", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238))
#define _tahoma825			gcnew System::Drawing::Font(L"Tahoma", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238))

//konwersje

#define _toISI		cli::safe_cast<System::ComponentModel::ISupportInitialize^>
#define _toImg		cli::safe_cast<System::Drawing::Image^>
#define _toIcon		cli::safe_cast<System::Drawing::Icon^>
#define _toString	cli::safe_cast<System::String^>
#define _toSysStr	gcnew System::String

//atrybuty

#define _autoScaleModeFont	System::Windows::Forms::AutoScaleMode::Font
#define _dockStyleNone		System::Windows::Forms::DockStyle::None
#define _alignMiddleLeft	System::Drawing::ContentAlignment::MiddleLeft
#define _alignMiddleRight	System::Drawing::ContentAlignment::MiddleRight

//obiekty

#define _dSize	System::Drawing::Size
#define _dSizeF	System::Drawing::SizeF
#define _dPoint	System::Drawing::Point

#define _componentResourceManager	System::ComponentModel::ComponentResourceManager
#define _container					System::ComponentModel::Container
#define _iContainer					System::ComponentModel::IContainer
#define _commandLine				System::Environment::CommandLine
#define _component					System::ComponentModel::Component

#define	_dir						System::IO::Directory
#define _dirInfo					System::IO::DirectoryInfo
#define _fileInfo					System::IO::FileInfo
#define _fileSysInfo				System::IO::FileSystemInfo
#define _ioPath						System::IO::Path

//kontrolki

#define _toolStripMenuItem	System::Windows::Forms::ToolStripMenuItem
#define _menuStrip			System::Windows::Forms::MenuStrip

#define _panel		System::Windows::Forms::Panel
#define _pictureBox	System::Windows::Forms::PictureBox
#define _label		System::Windows::Forms::Label

#define _toolStripItem			System::Windows::Forms::ToolStripItem
#define _toolStripStatusLabel	System::Windows::Forms::ToolStripStatusLabel
#define _toolStripProgressBar	System::Windows::Forms::ToolStripProgressBar
#define _statusStrip			System::Windows::Forms::StatusStrip

//przestrzenie nazw

#define _marshal	System::Runtime::InteropServices::Marshal
#define _fAtr		System::IO::FileAttributes

#endif

/************************************************/