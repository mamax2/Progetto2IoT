using System;
using System.IO.Ports;
using System.Windows;
using System.Windows.Threading;


namespace DASHBOARD_OPERATORE
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
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
                InitializeSerialPort(PORT);
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

        private void InitializeSerialPort(string portName)
        {
            if (!IsPortAvailable(portName))
            {
                throw new Exception("La porta specificata non è disponibile.");
            }

            serialPort = new SerialPort(portName, 9600);
            serialPort.DataReceived += SerialPort_DataReceived;
            serialPort.Open();
        }

        private bool IsPortAvailable(string portName)
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

        private void btnEmpty_Click(object sender, RoutedEventArgs e)
        {
            if (serialPort.IsOpen && serialPort != null)
            {
                serialPort.WriteLine("EMPTY");
                LogMessage("Sent to Arduino: EMPTY");
            }
        }

        private void btnRestore_Click(object sender, RoutedEventArgs e)
        {
            if (serialPort.IsOpen && serialPort != null)
            {
                serialPort.WriteLine("RESTORE");
                LogMessage("Sent to Arduino: RESTORE");
            }
        }

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



        // Chiudi la porta seriale quando l'app viene chiusa
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

        private void LogMessage(string message)
        {
            LogTextBox.AppendText(message + "\n");
            LogTextBox.ScrollToEnd();
        }

        private void btnStopReceiving_Click(object sender, RoutedEventArgs e)
        {
            isReceiving = !isReceiving;
            btnStopReceiving.Content = isReceiving ? "Stop Ricezione" : "Start Ricezione";
            LogMessage($"Ricezione {(isReceiving ? "attivata" : "disattivata")}");
        }
    }
}
