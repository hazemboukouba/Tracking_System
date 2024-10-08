#include <TinyGPS++.h> // bibliothèque de GPS 
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
TinyGPSPlus gps; 
static const int RXPin = 5, TXPin = 4;   // GPIO 4=D2(conneect Tx de GPS) and GPIO 5=D1(Connect Rx de GPS

SoftwareSerial nss(RXPin, TXPin);// la bibliothèque «SoftwareSerial» pour créer un port série virtuel.
 // La connexion série à l'appareil GPS
const char* nom = "Hazem"; //nom de  wifi
const char* mode_passe = "1234567890"; //mode passe de  wifi
float latitude , longitude;
int year, month, date, hour, minute, second;
String date_str , time_str , lat_str , lng_str;
int pm;
WiFiServer server(80);//le numéro de port du serveur Web. Pour la connexion HTTP , le numéro de port par défaut est 80.

void setup()
{
  Serial.begin(115200);
  nss.begin(9600);
  Serial.println();
  Serial.print(" Connexion à ");
  Serial.println(nom);
  WiFi.begin(nom, mode_passe); //connexion au wifi
  while (WiFi.status() != WL_CONNECTED)// si le wifi n'est pas connecté
  {
    delay(500);
    Serial.print("."); //ecrire "...."
  }
  Serial.println("");
  Serial.println("WiFi connectee");
  server.begin();
  Serial.println("Serveur démarré");
  Serial.println(WiFi.localIP());  // Imprimer l'adresse IP
}


void loop()
{
  while (nss.available() > 0) //pendant que les données sont disponibles
    if (gps.encode(nss.read())) //lire les données de gps
    {
      if (gps.location.isValid()) //vérifier si l'emplacement GPS est valide

      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6); // l'emplacement de la latitude est stocké dans une chaîne
        longitude = gps.location.lng();
        lng_str = String(longitude , 6); //l'emplacement de longitude est stocké dans une chaîne
      }
      if (gps.date.isValid()) //vérifier si la date GPS est validee
      {
        date_str = "";
        date = gps.date.day();
       month= gps.date.month();
       year = gps.date.year();
        if (date < 10)
          date_str = '0';
        date_str += String(date);// les valeurs de date, mois et année sont stockées dans une chaîne
        date_str += " / ";

        if ( month < 10)
          date_str += '0';
        date_str += String(month); //les valeurs de date, mois et année sont stockées dans une chaîne
        date_str += " / ";
        if (year< 10)
          date_str += '0';
        date_str += String(year); // les valeurs de date, mois et année sont stockées dans une chaîne
      }
      if (gps.time.isValid())  //vérifier si l'heure de GPS est validee
      {
        time_str = "";
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();
        hour = (hour + 1) ;
        if (hour > 23)
          hour = hour - 24;   // conversion de temps 
        if (hour >= 12)  // vérifier si AM ou PM
          pm = 1;
        else
          pm = 0;
        hour = hour % 12;
        if (hour < 10)
          time_str = '0';
        time_str += String(hour); //les valeurs d'heure, de minute et d'heure sont stockées dans une chaîne
        time_str += " : ";
        if (minute < 10)
          time_str += '0';
        time_str += String(minute); //les valeurs d'heure, de minute et d'heure sont stockées dans une chaîne
        time_str += " : ";
        if (second < 10)
          time_str += '0';
        time_str += String(second); //les valeurs d'heure, de minute et d'heure sont stockées dans une chaîne
        if (pm == 1)
          time_str += " PM ";
        else
          time_str += " AM ";
      }
    }
 
 WiFiClient client = server.available(); // nous vérifions si un client est connecté au serveur Web 
  if (!client)
  {
    return;

  }  
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>localisation</title> <style>";



s += "* { box-sizing: border-box;}";
s += "body {font-family: Arial, Helvetica, sans-serif;}";
s += "header {background-color: #666; padding: 30px; text-align: center; font-size: 18px; color: white;}";
s += "nav { float: left; width: 30%; height: 300px; background: #ccc; padding: 20px; }";
s += "article { float: left; padding: 20px; width: 100%; background-color: #f1f1f1; height: 300px;}";
s += "section:after {content: ""; display: table; clear: both;}";
s += "footer { background-color: #777; padding: 10px; text-align: center; color: white;}";
s += "a:link {background-color: YELLOW;text-decoration: none;}";
s += "table, th, td {border: 1px solid black;} </style> </head> <body> <h1 style=";
s += "font-size:300%;";
s += " ALIGN=CENTER> AGENCE DE LOCATION </h1>";

s += "<header>";
s += "<center>";
s += "<table>";
s += "<tr>";

s += " <td><h2>VOITURE_1</h2> </td>";

s += "</tr>";
s += "</table>";
s += "<center>";
s += "</header>";

s += "<section>";


s += "<article>";
s += "<h1>localisation du voiture_1 est :</h1>";
s += "<p ALIGN=CENTER style=""font-size:150%;""";
s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
s += "width:50%";
s += "> <tr> <th>Latitude</th>";
s += "<td ALIGN=CENTER >";
s += lat_str;
s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
s += lng_str;
s += "</td> </tr> <tr> <th>Date</th> <td ALIGN=CENTER >";
s += date_str;
s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
s += time_str;
s += "</td> </tr> </table> ";


if (gps.location.isValid())
{
s += "<p align=center><a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
s += lat_str;
s += "+";
s += lng_str;
s += """ target=""_top"">Cliquez ici!</a> To check the location in Google maps.</p>";
}
s += " </article>";
s += "</section>";

s += "<footer>";
s += " <h3> BOUKOUBA HAZEM </h3>";
//s += "</footer>";

s += "</body> </html> \n";


  client.print(s); // all the values are send to the webpage
  delay(100);
}
