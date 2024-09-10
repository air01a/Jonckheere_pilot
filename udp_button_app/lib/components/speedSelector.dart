import 'package:flutter/material.dart';

class SpeedSelector extends StatefulWidget {

  List<bool> _speedSelections = [true, false, false,false];
  @override
  State<SpeedSelector> createState() => _SpeedSelector();
}

class _SpeedSelector extends State<SpeedSelector> {
  @override
  Widget build(BuildContext context) {
    return Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          ToggleButtons(
            isSelected: widget._speedSelections,
            children: <Widget>[
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 20),
                child: Text('x1'),
              ),
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 20),
                child: Text('x2'),
              ),
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 20),
                child: Text('x4'),
              ),
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 20),
                child: Text('x16'),
              ),
            ])
            /*onPressed: (int index) {
              setState(() {
                for (int i = 0; i < _speedSelections.length; i++) {
                  _speedSelections[i] = i == index;  // Seul un bouton peut être sélectionné
                }
                switch (index) {
                  case 0:
                    _speedSelectedOption = 'x1';
                    break;
                  case 1:
                    _speedSelectedOption = 'x2';
                    break;
                  case 2:
                    _speedSelectedOption = 'x4';
                    break;
                  case 3:
                    _speedSelectedOption = 'x16';
                    break;
                };
                communicator.sendTcpMessage(_speedSelectedOption, serverIp, serverPort);
              });
            },*/
        ]);
  }

}