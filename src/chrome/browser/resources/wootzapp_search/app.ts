import './strings.m.js';

// import {loadTimeData} from 'chrome://resources/js/load_time_data.js';
// import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';
// import {getTemplate} from './app.html.js';

export class WootzappSearchAppElement extends HTMLBaseElement {
  static get is() {
    return 'wootzapp-search-app';
  }

  protected message_: string = "Hello Balram ";
}

declare global {
  interface HTMLElementTagNameMap {
    'wootzapp-search-app': WootzappSearchAppElement;
  }
}

customElements.define(WootzappSearchAppElement.is, WootzappSearchAppElement);