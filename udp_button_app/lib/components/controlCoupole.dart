import 'package:flutter/material.dart';
import '../controller/controller.dart';

class ControlCoupole extends StatefulWidget {
  Controller control;
  
  ControlCoupole({super.key, required this.control});

  @override
  _ControlcoupoleState createState() => _ControlcoupoleState();
}

class _ControlcoupoleState extends State<ControlCoupole> { 
 

  @override
  void initState(){
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children:<Widget>[
          

          Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [

             GestureDetector(
            onTapDown: (_) {
              widget.control.moveCoupole(-1);
            },
            onTapUp: (_) {
              widget.control.moveCoupole(0);
            },
            child: Container(
              margin: EdgeInsets.all(20),
              padding: EdgeInsets.symmetric(horizontal: 40, vertical: 20),
              color: Colors.blue,
              child: Text(
                'COU -',
                style: TextStyle(color: Colors.white, fontSize: 18),
              ),
            ),
          ),
          
           GestureDetector(
            onTapDown: (_) {
             widget.control.moveCoupole(1);
            },
            onTapUp: (_) {
              widget.control.moveCoupole(0);
            },
            child: Container(
              margin: EdgeInsets.all(20),
              padding: EdgeInsets.symmetric(horizontal: 40, vertical: 20),
              color: Colors.blue,
              child: Text(
                'COU+',
                style: TextStyle(color: Colors.white, fontSize: 18),
              ),
            ),
          ),
          ],),

           ]);
  }
  


}