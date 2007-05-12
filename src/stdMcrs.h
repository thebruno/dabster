/*********************************************************************

	Sigma Dabster 5
	Copyright (C) The Dabster Team 2007.
	All rights reserved.

	http://www.dabster.prv.pl/
	http://code.google.com/p/dabster/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 dated June, 1991. 

	This program is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	General Public License for more details. 

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
	02110-1301 USA. 

	Standard Macros Header
	stdMcrs.h

*********************************************************************/

#ifndef _STD_MCRS_H_
#define _STD_MCRS_H_

//kolory

#define dabWhite			System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), \
															 static_cast<System::Int32>(static_cast<System::Byte>(255)), \
														 	 static_cast<System::Int32>(static_cast<System::Byte>(255)))
#define dabBlack			System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), \
														 	 static_cast<System::Int32>(static_cast<System::Byte>(0)), \
														 	 static_cast<System::Int32>(static_cast<System::Byte>(0)))
#define dabLightBrown		System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(236)), \
														 	 static_cast<System::Int32>(static_cast<System::Byte>(233)), \
														 	 static_cast<System::Int32>(static_cast<System::Byte>(216)))
#define dabLightBlue		System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(187)), \
														 	 static_cast<System::Int32>(static_cast<System::Byte>(211)), \
														 	 static_cast<System::Int32>(static_cast<System::Byte>(249)))
#define dabDarkBlue			System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(49)), \
														 	 static_cast<System::Int32>(static_cast<System::Byte>(106)), \
														 	 static_cast<System::Int32>(static_cast<System::Byte>(197)))
#define dabTransparent		System::Drawing::Color::Transparent

//czcionki

#define dabSansSerif825			gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, \
															System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238))
#define dabSansSerif825Bold		gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, \
															System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238))
#define dabArial9				gcnew System::Drawing::Font(L"Arial", 9, System::Drawing::FontStyle::Regular, \
															System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238))
#define dabTahoma825			gcnew System::Drawing::Font(L"Tahoma", 8.25F, System::Drawing::FontStyle::Regular, \
															System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(238))

//konwersje

#define dabToISI		cli::safe_cast<System::ComponentModel::ISupportInitialize^>
#define dabToImg		cli::safe_cast<System::Drawing::Image^>
#define dabToIcon		cli::safe_cast<System::Drawing::Icon^>
#define dabToString		cli::safe_cast<System::String^>
#define dabToSysStr		gcnew System::String

//atrybuty

#define dabAutoScaleModeFont	System::Windows::Forms::AutoScaleMode::Font
#define dabDockStyleNone		System::Windows::Forms::DockStyle::None
#define dabAlignMiddleLeft		System::Drawing::ContentAlignment::MiddleLeft
#define dabAlignMiddleRight		System::Drawing::ContentAlignment::MiddleRight

//obiekty

#define dabDSize	System::Drawing::Size
#define dabDSizeF	System::Drawing::SizeF
#define dabDPoint	System::Drawing::Point

#define dabComponentResourceManager		System::ComponentModel::ComponentResourceManager
#define dabContainer					System::ComponentModel::Container
#define dabIContainer					System::ComponentModel::IContainer
#define dabCommandLine					System::Environment::CommandLine
#define dabComponent					System::ComponentModel::Component

#define	dabDir				System::IO::Directory
#define dabDirInfo			System::IO::DirectoryInfo
#define dabFile				System::IO::File
#define dabFileInfo			System::IO::FileInfo
#define dabFileSysInfo		System::IO::FileSystemInfo
#define dabIoPath			System::IO::Path

//kontrolki

#define dabToolStripMenuItem	System::Windows::Forms::ToolStripMenuItem
#define dabMenuStrip			System::Windows::Forms::MenuStrip

#define dabPanel		System::Windows::Forms::Panel
#define dabPictureBox	System::Windows::Forms::PictureBox
#define dabLabel		System::Windows::Forms::Label

#define dabToolStripItem			System::Windows::Forms::ToolStripItem
#define dabToolStripStatusLabel		System::Windows::Forms::ToolStripStatusLabel
#define dabToolStripProgressBar		System::Windows::Forms::ToolStripProgressBar
#define dabStatusStrip				System::Windows::Forms::StatusStrip

//przestrzenie nazw

#define dabMarshal	System::Runtime::InteropServices::Marshal
#define dabFAtr		System::IO::FileAttributes

//klucze (i ich wartosci) uzywane do przekazywania informacji o obiektach DFB

#define dabKeyName					"name"
#define dabKeyRealPath				"realPath"
#define dabKeyRelativePath			"relativePath"
#define dabKeyVolumeLabel			"volumeLabel"
#define dabKeyDriveFormat			"driveFormat"
#define dabKeyDriveType				"driveType"
#define dabKeyIsReady				"isReady"
#define dabKeyAvailableFreeSpace	"availableFreeSpace"
#define dabKeyTotalFreeSpace		"totalFreeSpace"
#define dabKeyTotalSize				"totalSize"
#define dabKeyLength				"length"
#define dabKeyCreationTimeUtc		"creationTimeUtc"
#define dabKeyLastWriteTimeUtc		"lastWriteTimeUtc"
#define dabKeyLastAccessTimeUtc		"lastAccessTimeUtc"
#define dabKeyAtrTemporary			"atrTemporary"
#define dabKeyAtrSystem				"atrSystem"
#define dabKeyAtrSparseFile			"atrSparseFile"
#define dabKeyAtrArchive			"atrArchive"
#define dabKeyAtrCompressed			"atrCompressed"
#define dabKeyAtrDevice				"atrDevice"
#define dabKeyAtrEncrypted			"atrEncrypted"
#define dabKeyAtrHidden				"atrHidden"
#define dabKeyAtrNormal				"atrNormal"
#define dabKeyAtrNotContentIndexed	"atrNotContentIndexed"
#define dabKeyAtrOffline			"atrOffline"
#define dabKeyAtrReadOnly			"atrReadOnly"
#define dabKeyAtrReparsePoint		"atrReparsePoint"
#define dabKeyAtrDirectory			"atrDirectory"

#define dabFalse		"false"
#define dabTrue			"true"
#define dabUnknown		"unknown"

//Typy elementow drzewa

static const int dabFile =		0x200;
static const int dabFolder =	0x400;
static const int dabDrive =		0x800;

static const int dabTypeMask =	0xFF;

static const int dabSFile =		1 + dabFile;
static const int dabSFolder =	2			+ dabFolder;
static const int dabSDrive =	3			+ dabFolder	+ dabDrive;
static const int dabBmp =		4 + dabFile + dabFolder + dabDrive;
static const int dabAes =		5 + dabFile + dabFolder;
static const int dabBwt =		6 + dabFile + dabFolder;
static const int dabWav =		7 + dabFile + dabFolder + dabDrive;
static const int dabTwfsh =		8 + dabFile + dabFolder;
static const int dabFtp =		9			+ dabFolder + dabDrive;
static const int dabMldrv =		10			+ dabFolder + dabDrive;

#endif

/********************************************************************/