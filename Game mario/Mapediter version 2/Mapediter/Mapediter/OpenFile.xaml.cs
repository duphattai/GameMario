using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Mapediter
{
    /// <summary>
    /// Interaction logic for OpenFile.xaml
    /// </summary>
    public partial class OpenFile : Window
    {
        public string fileImagePath;
        public double scale;
        public int tileWidth, tileHeight;
        public int minWidth;
        public int mapWidth;
        public int mapHeight;
        public OpenFile()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog openfile = new Microsoft.Win32.OpenFileDialog();

            openfile.Filter = "All File | *.png; *.jpg; *.jpeg; *.gif";

            Nullable<bool> result = openfile.ShowDialog();

            if (result == true)
            {
                fileImagePath = openfile.FileName;
                PathImage.Text = fileImagePath;
            }
            else return;
        }

        private void Ok_Click(object sender, RoutedEventArgs e)
        {
            scale = Convert.ToDouble(Scale.Text.ToString());
            tileHeight = Convert.ToInt32(TileHeight.Text.ToString());
            tileWidth = Convert.ToInt32(TileWidth.Text.ToString());
            minWidth = Convert.ToInt32(Min_width.Text.ToString());
            mapHeight = Convert.ToInt32(MapHeight.Text.ToString());
            mapWidth = Convert.ToInt32(MapWidth.Text.ToString());
            this.Close();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
