import 'package:flutter/material.dart';
import '../controller/controller.dart';

class ControlAxis extends StatefulWidget {
  Controller control;

  ControlAxis({required this.control});
  @override
  _ControlAxisState createState() => _ControlAxisState();
}

class _ControlAxisState extends State<ControlAxis> { 
 bool endStroke = false;

  @override
  void initState(){
    super.initState();
    widget.control.addListener("EOStroke", onEndOfStroke);
  }


  void onEndOfStroke(){
    setState(() {
      endStroke=true;
    });
  }

  void resetEndOfStroke(){
    setState(() {
      endStroke=false;
    });
  }


  @override
  Widget build(BuildContext context) {
    return Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children:<Widget>[
          GestureDetector(
            onTapDown: (_) {
              resetEndOfStroke();

              widget.control.changeAxisSpeed(1,1);
            },
            onTapUp: (_) {
              widget.control.changeAxisSpeed(1,0);
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
              widget.control.changeAxisSpeed(0,-1);
            },
            onTapUp: (_) {
              widget.control.changeAxisSpeed(0,0);
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
          Container(
                width: 30,
                height: 30,
                decoration: BoxDecoration(
                  shape: BoxShape.circle,
                  color: endStroke ? Colors.red : Colors.green, // Rouge si problème, Vert sinon
                ),
              ),

           GestureDetector(
            onTapDown: (_) {
             widget.control.changeAxisSpeed(0,1);
            },
            onTapUp: (_) {
              widget.control.changeAxisSpeed(0,0);
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
              resetEndOfStroke();

              widget.control.changeAxisSpeed(1,-1);
            },
            onTapUp: (_) {
              widget.control.changeAxisSpeed(1,0);
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