import React from 'react';
import { Component } from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import App from './App';
import registerServiceWorker from './registerServiceWorker';

import '../node_modules/react-grid-layout/css/styles.css';
import '../node_modules/react-resizable/css/styles.css';

import Menu, { SubMenu, Item as MenuItem, Divider } from 'rc-menu';

        // import R from 'react-addons';
// import Box from 'react-box-layout';

var ReactGridLayout  = require('react-grid-layout');

// var ffi = require('ffi');

// SOL 1 //
var createReactClass = require('create-react-class');
var MyFirstGrid = createReactClass({
  render: function() {
    // layout is an array of objects, see the demo for more complete usage
    var layout = [
      {i: 'a   ', x: 0, y: 0, w: 1, h: 2, static: true},
      {i: 'b   ', x: 1, y: 0, w: 3, h: 2, minW: 2, maxW: 4},
      {i: 'c   ', x: 4, y: 0, w: 1, h: 2}
    ];

    return (
        <div>
        <Menu>
           <MenuItem>1</MenuItem>
          <SubMenu title="2">
          <MenuItem>2-1</MenuItem>
          </SubMenu>
        </Menu>
      <ReactGridLayout className="layout" layout={layout} cols={12} rowHeight={30} width={1200}>
        <div key="a">a</div>
        <div key="b">b</div>
        <div key="c">c</div>
      </ReactGridLayout>
        </div>
    )
  }
});
ReactDOM.render(<MyFirstGrid />, document.getElementById('root'));

// SOL 2 //
//class Gri extends Component {
//    render() {
//      // layout is an array of objects, see the demo for more complete usage
//      var layout = [
//        {i: 'a   ', x: 0, y: 0, w: 1, h: 2, static: true},
//        {i: 'b   ', x: 1, y: 0, w: 3, h: 2, minW: 2, maxW: 4},
//        {i: 'c   ', x: 4, y: 0, w: 1, h: 2}
//      ];

//      return (
//        <ReactGridLayout className="layout" layout={layout} cols={12} rowHeight={30} width={1200}>
//          <div key="a">a</div>
//          <div key="b">b</div>
//          <div key="c">c</div>
//        </ReactGridLayout>
//      )
//    }
//}
//ReactDOM.render(<Gri />, document.getElementById('root'));

registerServiceWorker();
