﻿/* 
 * Copyright (C) 2012 Jordi Mas i Hernàndez <jmas@softcatala.org>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
 * 02111-1307, USA.
 */
 
#pragma once

#include "Defines.h"
#include "XmlNode.h"
#include <string>
#import <msxml3.dll>
#include <vector>

using namespace std;
using namespace MSXML2;

typedef bool (*NodeCallback)(XmlNode node, void *data);

class XmlParser
{
public:
		XmlParser();
		~XmlParser();
		void Parse(NodeCallback callback, void *data);
		bool Load(wstring file);
		bool Save(wstring file);
		void AppendNode(XmlNode node);
		bool FindNode(wstring wfind, XmlNode& node);
		bool ReplaceNode(XmlNode nodeNew, XmlNode nodeOld);
		MSXML2::IXMLDOMDocumentPtr getDocument() const {return m_domDocument;}

		
private:
		void _initialize();
		void _uninitialize();
		void _parseNode(MSXML2::IXMLDOMNode *pIDOMNode, XmlNode& node);
		void _parseNodeGetText(MSXML2::IXMLDOMNode *pIDOMNode, XmlNode& node);
		void _parseNodeGetAttributes(MSXML2::IXMLDOMNode *pIDOMNode, XmlNode& node);		

		MSXML2::IXMLDOMDocumentPtr m_domDocument;
};