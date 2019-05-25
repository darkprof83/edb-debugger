/*
Copyright (C) 2006 - 2015 * Evan Teran evan.teran@gmail.com
                          * darkprof dark_prof@mail.ru

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FasLoader.hpp"
#include "OptionsPage.hpp"
#include "IDebugger.h"
#include "IProcess.h"
#include "ISymbolManager.h"
#include "Symbol.h"
#include "edb.h"

#include <QMenu>
#include <QSettings>
#include <iostream>

namespace FasLoaderPlugin {

FasLoader::FasLoader ( QObject* parent ) 
  : QObject ( parent )
{
  
}

void 
FasLoader::private_init () 
{
  // QSettings settings;
	// if(settings.value("FasLoader/load_on_start.enabled", true).toBool()) {
		// load();
	// }
}

QMenu*
FasLoader::menu(QWidget* parent) {

	Q_ASSERT(parent);

	if(!menu_) {
		menu_ = new QMenu(tr("FasLoader"), parent);
		menu_->addAction(tr("&Load *.fas symbols"), this, SLOT(show_menu()));
	}

	return menu_;
}

// QWidget*
// FasLoader::options_page() {
	// return new OptionsPage;
// }

void 
FasLoader::show_menu() {
	// initial_check_ = false;
	load();
}

void 
FasLoader::load () 
{
  if ( edb::v1::debugger_core ) {
    auto process = edb::v1::debugger_core->process ();
    if ( process ) {
      auto fileName = process->executable ();
      auto fasName = fileName;
      fasName.append ( ".fas" );
      fasCore.load ( fasName.toUtf8 ().constData () );
      auto pluginSymbols = fasCore.getSymbols ();
      for ( auto pluginSymbol : pluginSymbols ) {
        std::shared_ptr<Symbol> symbol ( new Symbol () );
        symbol->file = fileName;
        symbol->address = pluginSymbol.value;
        symbol->name = QString::fromStdString ( pluginSymbol.name );
        symbol->size = pluginSymbol.size;
        if ( pluginSymbol.size > 0 ) {
          symbol->type = 'd';
        }
        edb::v1::symbol_manager ().add_symbol ( symbol );
      }
    }
  }
}

}