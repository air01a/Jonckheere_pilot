import '../services/communicator.dart';

class Controller {

  final String serverIp = "10.113.211.167";//"192.168.1.1";  // Remplacez par l'adresse IP de votre serveur
  final int serverPort = 4000;

  List<bool> _selections = [true, false, false]; // Le premier est sélectionné par défaut
  List<bool> _speedSelections = [true, false, false,false]; // Le premier est sélectionné par défaut
  bool hasError = false;
  late Communicator communicator; 

  void changeSpeed(List<bool> selection) {

  }

}