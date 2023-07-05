const int sonarTrigPin[] = {3, 5, 7};
const int sonarEchoPin[] = {2, 4, 6};
const int semaforoVerdePin[] = {8, 12, 10};
const int semaforoVermelhoPin[] = {9, 13, 11};
int distMin = 20;
int tempoSemaforo = 10;
void setup()
{
  //Ativar os pinos do arduíno
  for (int i = 0; i < 3; i++) {
    pinMode(semaforoVerdePin[i], OUTPUT);
    pinMode(semaforoVermelhoPin[i], OUTPUT);
    digitalWrite(semaforoVermelhoPin[i], HIGH);
  }
  //Ativar os pinos do sensor
  for (int i = 0; i < 3; i++) {
    pinMode(sonarTrigPin[i], OUTPUT);
    pinMode(sonarEchoPin[i], INPUT);
  }
}

void triggerSensor(int trigPin)
{
  //Ativar o sensor
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
}
int verificadorSensor(){ //verifica se há carro em algum outro sinal
  int cont = 0; // inicializa contador
  for (int i = 0; i < 3; i++) {
    triggerSensor(sonarTrigPin[i]); //Utiliza a função para ativar o sensor
    int distancia = 0.01723 * pulseIn(sonarEchoPin[i], HIGH); //determina a distancia entre o carro e o sensor
    if (distancia <= distMin) {  //condicional para adicionar o semáforo na lista
      cont += 1;// adiciona +1 ao contador se a distancia for menor do que 100
    }
  }
  return cont;
}
//funcionamento do semáforo
void acenderSinal(int semaforoVerde, int semaforoVermelho, int trigPin, int echoPin)
{
  digitalWrite(semaforoVermelho, LOW);
  int cont = 0; // inicializa outro contado
  int verif = 1; //ativa o sensor
  triggerSensor(trigPin);
  //looping do semáforo
  while (verif == 1 && 0.01723 * pulseIn(echoPin, HIGH) <= distMin){
    digitalWrite(semaforoVerde, HIGH);
    delay(1000);
	cont += 1;
    triggerSensor(trigPin); //Utiliza a função para ativar o sensor
    if (cont == tempoSemaforo){
    	verif = verificadorSensor(); // utiliza o verificador a cada 10 segundos para ver se há algum carro
    }
  }
  digitalWrite(semaforoVerde, LOW); // apaga sinal verde
  digitalWrite(semaforoVermelho, HIGH); // acende sinal vermelho
  delay(5000);
}

void loop()
  //looping principal
{
  int dadosSemaforo[] = {-1, -1, -1}; //Lista para armazenar os dados dos semáforos que serão utilizados
  //Estrutura de repetição para verificar a distancia entre o carro e o sensor
  for (int i = 0; i < 3; i++) {
    triggerSensor(sonarTrigPin[i]); //Utiliza a função para ativar o sensor
    int distancia = 0.01723 * pulseIn(sonarEchoPin[i], HIGH); //determina a distancia entre o carro e o sensor
    if (distancia <= distMin) {  //condicional para adicionar o semáforo na lista
      dadosSemaforo[i] = semaforoVerdePin[i]; //Adicionando led verde na lista
    }
  }

  for (int i = 0; i < 3; i++) { // estrutura de repetição para percorrer a lista
    if (dadosSemaforo[i] > -1) { //Condicional para os dados do semáforo ser processados
      acenderSinal(dadosSemaforo[i], dadosSemaforo[i]+1, sonarTrigPin[i], sonarEchoPin[i]);//Função para processamento do semáforo
    }
  }
}
