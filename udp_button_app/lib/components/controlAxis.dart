import 'package:flutter/material.dart';

class ControlAxis extends StatelessWidget {
  Function sendControl;

  ControlAxis({required this.sendControl});

  @override
  Widget build(BuildContext context) {
    return Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children:<Widget>[
          GestureDetector(
            onTapDown: (_) {
              sendControl(1,1);
            },
            onTapUp: (_) {
              sendControl(1,0);
            },
            child: Container(
              margin: EdgeInsets.all(20),
              padding: EdgeInsets.symmetric(horizontal: 40, vertical: 20),
              color: Colors.blue,
              child: Text(
                'DEC+',
                style: TextStyle(color: Colors.white, fontSize: 18),
              ),
            ),
          ),

          Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [

             GestureDetector(
            onTapDown: (_) {
              sendControl(0,-1);
            },
            onTapUp: (_) {
              sendControl(0,0);
            },
            child: Container(
              margin: EdgeInsets.all(20),
              padding: EdgeInsets.symmetric(horizontal: 40, vertical: 20),
              color: Colors.blue,
              child: Text(
                'AD-',
                style: TextStyle(color: Colors.white, fontSize: 18),
              ),
            ),
          ),

           GestureDetector(
            onTapDown: (_) {
             sendControl(0,1);
            },
            onTapUp: (_) {
              sendControl(0,0);
            },
            child: Container(
              margin: EdgeInsets.all(20),
              padding: EdgeInsets.symmetric(horizontal: 40, vertical: 20),
              color: Colors.blue,
              child: Text(
                'AD+',
                style: TextStyle(color: Colors.white, fontSize: 18),
              ),
            ),
          ),
          ],),

           GestureDetector(
            onTapDown: (_) {
              sendControl(1,-1);
            },
            onTapUp: (_) {
              sendControl(1,0);
            },
            child: Container(
              margin: EdgeInsets.all(20),
              padding: EdgeInsets.symmetric(horizontal: 40, vertical: 20),
              color: Colors.blue,
              child: Text(
                'DEC-',
                style: TextStyle(color: Colors.white, fontSize: 18),
              ),
            ),
          )]);
  }
  


}