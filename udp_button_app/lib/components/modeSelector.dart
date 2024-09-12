import 'package:flutter/material.dart';

class ModeSelector extends StatefulWidget {
  final Function(List<bool>) onValueChanged;
  List<bool> modeSelected ;

  ModeSelector({super.key, required this.modeSelected, required this.onValueChanged});

  @override
  State<ModeSelector> createState() => _SpeedSelector();
}

class _SpeedSelector extends State<ModeSelector> {
  @override
  Widget build(BuildContext context) {
    return Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          ToggleButtons(
            isSelected: widget.modeSelected,
            onPressed: (int index) {
              setState(() {
                for (int i = 0; i < widget.modeSelected.length; i++) {
                  widget.modeSelected[i] = i == index;  // Seul un bouton peut être sélectionné
                }
                widget.onValueChanged(widget.modeSelected);
              });
            },
            children: const <Widget>[
              Padding(
                padding:  EdgeInsets.symmetric(horizontal: 20),
                child: Text('Sidereal'),
              ),
              Padding(
                padding:  EdgeInsets.symmetric(horizontal: 20),
                child: Text('Lunar'),
              ),
               Padding(
                padding:  EdgeInsets.symmetric(horizontal: 20),
                child:  Text('Solar'),
              ),
            ],
          )]);
  }
}