import '../services/communicator.dart';

class Controller {

  final String serverIp = "192.168.1.1";//"192.168.1.1";  // Remplacez par l'adresse IP de votre serveur
  final int serverPort = 4000;

  List<bool> modeSelections = [true, false, false]; // Le premier est sélectionné par défaut
  List<bool> speedSelections = [true, false, false,false]; // Le premier est sélectionné par défaut
  List<String> possibleSpeed = ["x1", "x2", "x4", "x16"];
  List<String> possibleMode = ["sidereal","lunar","solar"];
  late Communicator communicator; 
  final Function onStateChanged;

  Controller(this.onStateChanged) {
    communicator = Communicator(preSendFunction: preSendFunction, postSendFunction: manageResult, debug:addDebugMessage);
    communicator.sendTcpMessage("getParams", serverIp, serverPort);
  }


 void addDebugMessage(String message){

 }

 void preSendFunction() {
     onStateChanged(true);

  }

 void manageResult(String message) {
    // Simule une opération asynchrone (ex : requête réseau, lecture de fichier, etc.)
    //await sendTcpMessage("getParams", serverIp, serverPort);  // Attente de 2 secondes pour la simulation
    List<String> segments = message.split('_');
    // Lorsque l'opération asynchrone est terminée, mettre à jour les données
    if (segments[0]=="OK") {
          onStateChanged(false);
    };
    // Mettre à jour l'interface utilisateur avec les nouvelles données
      if (segments.length==3) {
          String _selectedMode=segments[1];
          String _speedSelectedOption=segments[2];

          switch (_selectedMode) {
            case "sidereal":
              modeSelections=[true,false,false];
              break;
            case "lunar":
              modeSelections=[false,true,false];
              break;
            case "solar":
              modeSelections=[false,false,true];
              break;

          }

          switch (_speedSelectedOption) {
            case "x1":
              speedSelections=[true,false,false,false];
              break;
            case "x2":
              speedSelections=[false,true,false,false];
              break;
            case "x4":
              speedSelections=[false,false,true,false];
              break;
            case "x16":
              speedSelections=[false,false,false,true];
              break;
          }
          onStateChanged(false);
      }
  }

  void changeSpeed(List<bool> selection) {
    speedSelections = selection; 
    int index = selection.indexOf(true);
    if (index != -1) {
      String cmd = possibleSpeed[index];

      communicator.sendUdpMessage(cmd, serverIp, serverPort);
    }
  }

  void changeMode(List<bool> selection) {
    modeSelections = selection; 
    int index = selection.indexOf(true);
    if (index != -1) {
      String cmd = possibleMode[index];
      communicator.sendUdpMessage(cmd, serverIp, serverPort);
    }
  }


  void changeAxisSpeed(int axis, int orientation) {
    String cmd;
    if (axis==0) {
      cmd="AD";
    } else {
      cmd="DEC";
    }
    
    if (orientation==-1) {
      cmd+="-";
    }
    if (orientation==1) {
      cmd+="+";
    }
    

    communicator.sendUdpMessage(cmd, serverIp, serverPort);
  }
}