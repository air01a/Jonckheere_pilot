import 'package:flutter/material.dart';

class SpeedSelector extends StatefulWidget {
  final Function(List<bool>) onValueChanged;
  List<bool> speedSelections ;

  SpeedSelector({super.key, required this.speedSelections, required this.onValueChanged});

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
            isSelected: widget.speedSelections,
            children: const <Widget>[
              Padding(
                padding: EdgeInsets.symmetric(horizontal: 20),
                child: Text('x1'),
              ),
              Padding(
                padding: EdgeInsets.symmetric(horizontal: 20),
                child: Text('x2'),
              ),
              Padding(
                padding: EdgeInsets.symmetric(horizontal: 20),
                child: Text('x4'),
              ),
              Padding(
                padding: EdgeInsets.symmetric(horizontal: 20),
                child: Text('x16'),
              ),
            ],
            onPressed: (int index) {
              setState(() {
                  for (int i = 0; i < widget.speedSelections.length; i++) {
                    widget.speedSelections[i] = i == index;  // Seul un bouton peut être sélectionné
                  }
                  widget.onValueChanged(widget.speedSelections);

                });
            })
        ]);   
  }

}