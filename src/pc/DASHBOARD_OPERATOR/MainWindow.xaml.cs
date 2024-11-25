using System;
using System.IO.Ports; // Libreria per gestire la porta seriale.
using System.Windows;
using System.Windows.Threading;


namespace DASHBOARD_OPERATORE
{
    /// <summary>
    /// Dashboard dell'operatore.
    /// </summary>
    public partial class MainWindow : Window
    {
        private SerialPort serialPort;
        private const string PORT = "COM6";
        private bool isReceiving = true;


        public MainWindow()
        {
            InitializeComponent();
            try
            {
                InitializeSerialPort();
                _ = MessageBox.Show($"Connesso ad Arduino su {PORT}.", "Connessione Riuscita", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            catch (Exception ex)
            {
                _ = MessageBox.Show($"Impossibile connettersi alla porta {PORT}. Assicurati che Arduino sia connesso.\n\nErrore: {ex.Message}",
                    "Errore di Connessione",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error);
            }

        }

        /// <summary>
        /// Metodo per inizializzare la comunicazione sulla porta seriale passata come argomento.
        /// </summary>
        private void InitializeSerialPort()
        {
            if (!IsPortAvailable())
            {
                throw new Exception("La porta specificata non è disponibile."); // Se le porta non è disponibile genera un'eccezione.
            }
            serialPort = new SerialPort(PORT, 9600);
            serialPort.DataReceived += SerialPort_DataReceived;
            serialPort.Open();
        }

        /// <summary>
        /// Metodo che controlla se la porta seriale è disponibile.
        /// </summary>
        /// <returns></returns>
        private bool IsPortAvailable()
        {
            string[] availablePorts = SerialPort.GetPortNames();
            foreach (string port in availablePorts)
            {
                if (port.Equals(PORT))
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// Evento che si verifica quando viene premuto il pulsante empty.
        /// Verifica se la porta seriale è aperta e non nulla e poi manda il comando ad arduino + un messaggio alla textbox per verificare 
        /// l'effettivo successo dell' invio.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnEmpty_Click(object sender, RoutedEventArgs e)
        {
            if (serialPort.IsOpen && serialPort != null)
            {
                serialPort.WriteLine("EMPTY");
                LogMessage("Sent to Arduino: EMPTY");
            }
        }

        /// <summary>
        /// Evento che si verifica quando viene premuto il pulsante restore.
        /// Verifica se la porta seriale è aperta e non nulla e poi manda il comando ad arduino + un messaggio alla textbox per verificare 
        /// l'effettivo successo dell' invio.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnRestore_Click(object sender, RoutedEventArgs e)
        {
            if (serialPort.IsOpen && serialPort != null)
            {
                serialPort.WriteLine("RESTORE");
                LogMessage("Sent to Arduino: RESTORE");
            }
        }

        /// <summary>
        /// Evento che gestisce l'arrivo di dati da parte di arduino.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (!isReceiving)
            {
                return; // Se la ricezione è disabilitata esci 
            }

            try
            {
                string data = serialPort.ReadLine().Trim(); // Legge una linea e rimuove spazi bianchi
                Dispatcher.Invoke(() =>
                {
                    LogMessage($"Received from Arduino: {data}");
                    if (data.StartsWith("Temperature:"))
                    {
                        string temperatureString = data.Split(':')[1];
                        if (float.TryParse(temperatureString, out float temperature))
                        {
                            TemperatureText.Text = $"{temperature}°C";
                        }
                    }
                    else if (data.StartsWith("LevelOfWaste:"))
                    {
                        string wasteValueString = data.Split(':')[1];
                        if (int.TryParse(wasteValueString, out int wasteValue))
                        {
                            FillLevelProgressBar.Value = wasteValue;
                            FillLevelText.Text = wasteValueString;
                        }
                    }
                    else
                    {
                        LogMessage($"Unrecognized data: {data}");
                    }
                });
            }
            catch (Exception ex)
            {
                Dispatcher.Invoke(() =>
                {
                    LogMessage($"Error: {ex.Message}");
                });
            }
        }



        /// <summary>
        /// Evento per chiudere la porta seriale alla chiusura del programma.
        /// </summary>
        /// <param name="e"></param>
        protected override void OnClosed(EventArgs e)
        {
            if (serialPort != null)
            {
                if (serialPort.IsOpen)
                {
                    serialPort.Close();
                }
            }
            base.OnClosed(e);
        }

        /// <summary>
        /// Metodo per mostrare i comandi mandati e ricevuti da arduino.
        /// </summary>
        /// <param name="message"></param>
        private void LogMessage(string message)
        {
            LogTextBox.AppendText(message + "\n");
            LogTextBox.ScrollToEnd();
        }

        /// <summary>
        /// Pulsante di debug per fermare la ricezione di comandi da parte di arduino.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnStopReceiving_Click(object sender, RoutedEventArgs e)
        {
            isReceiving = !isReceiving;
            btnStopReceiving.Content = isReceiving ? "Stop Ricezione" : "Start Ricezione";
            LogMessage($"Ricezione {(isReceiving ? "attivata" : "disattivata")}");
        }
    }
}
