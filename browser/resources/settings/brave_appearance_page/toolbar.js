// Copyright (c) 2020 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import { Polymer, html } from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';
import { loadTimeData } from "../i18n_setup.js"
import '../settings_shared_css.js';
import '../settings_vars_css.js';

/**
 * 'settings-brave-appearance-toolbar' is the settings page area containing
 * brave's appearance settings related to the toolbar.
 */
Polymer({
  is: 'settings-brave-appearance-toolbar',

  _template: html`{__html_template__}`,

  properties: {
    tabTooltipModes_: {
      readOnly: true,
      type: Array,
      value() {
        return [
          { value: 1, name: 'Card' },
          { value: 2, name: 'Card with preview' },
          { value: 0, name: 'Classic' }
        ]
      }
    }
  },

  showBraveVPNOption_: function () {
    return loadTimeData.getBoolean('isBraveVPNEnabled');
  },

  onTabTooltipModeChange_() {
    console.log("Tooltip Mode:", this.$.braveTooltipMode.value)
  },

  getCurrentTabTooltipMode_() {
    return this.get('prefs.brave.tabs.tooltip_mode');
  }
});

