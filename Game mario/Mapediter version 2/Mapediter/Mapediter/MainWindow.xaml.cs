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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Mapediter
{
    class MyRectangle
    {
        public int id;
        public int x, y;
        public int index;
        public Rectangle rect;

        public MyRectangle() { rect = new Rectangle(); }

        public MyRectangle(int id, int index, int x, int y, Rectangle rect)
        {
            this.id = id;
            this.index = index;
            this.rect = rect;
        }
    }
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 
    public partial class MainWindow : Window
    {
        int ScaleBackGround;
        int tileWidth, tileHeight;
        int mapWidth, mapHeight;

        string m_TextImage;
        TileMap m_TileMap = null;
        string m_Information;

        int currentIndex;

        List<string> m_ListInfor = new List<string>();
        List<MyRectangle> m_ListNode = new List<MyRectangle>();// store position of rectangle draw on canvas
        List<Line> listGrid = new List<Line>(); // store lines will draw on canvas 

        public static int minWidth; // use for quadtree
        public MainWindow()
        {
            InitializeComponent();
            mycanvas.Focus();
            Keyboard.Focus(mycanvas);
        }
  
        private void new_Click(object sender, RoutedEventArgs e)
        {
            OpenFile form = new OpenFile();
            form.ShowDialog();

            if (string.IsNullOrEmpty(form.fileImagePath)) return;

            ScaleBackGround = (int)form.scale;
            m_TextImage = form.fileImagePath;
            tileHeight = form.tileHeight;
            tileWidth = form.tileWidth;
            minWidth = form.minWidth;
            mapWidth = form.mapWidth;
            mapHeight = form.mapHeight;


            ImageBrush img = new ImageBrush();
            img.ImageSource = new BitmapImage(new Uri(m_TextImage));

            mycanvas.Width = form.mapWidth * ScaleBackGround;
            mycanvas.Height = form.mapHeight * ScaleBackGround;

            mycanvasTileMap.Width = img.ImageSource.Width;
            mycanvasTileMap.Height = img.ImageSource.Height;
            mycanvasTileMap.Background = img;


            //draw grid
            double x = tileWidth;
            double y = tileHeight;
            
            for (int i = 0; i < mycanvasTileMap.Width / x; i++)
            {
                Line line = new Line();
                line.Stroke = System.Windows.Media.Brushes.LightSteelBlue;

                line.X1 = i * x;
                line.Y1 = 0;
                line.X2 = line.X1;
                line.Y2 = mycanvasTileMap.Height;

                mycanvasTileMap.Children.Add(line);
            }

            for (int i = 0; i < mycanvasTileMap.Height / y; i++)
            {
                Line line = new Line();
                line.Stroke = System.Windows.Media.Brushes.LightSteelBlue;

                line.X1 = 0;
                line.Y1 = (double)i * y;
                line.X2 = mycanvasTileMap.Width;
                line.Y2 = line.Y1;

                mycanvasTileMap.Children.Add(line);
            }
        }

        private void createTiledMap_Click(object sender, RoutedEventArgs e)
        {
            List<Node> list = new List<Node>();

            foreach(var item in m_ListNode)
            {
                Node temp = new Node(item.id, item.index, (int)item.x / ScaleBackGround, (int)item.y / ScaleBackGround, (int)item.rect.Width / ScaleBackGround, (int)item.rect.Height / ScaleBackGround);
                list.Add(temp);
            }


            foreach(var item in m_ListInfor)
            {
                m_Information += item + Environment.NewLine;
            }


            m_TileMap = new TileMap(list, m_Information, tileWidth, tileHeight, mapWidth, mapHeight);
            m_TileMap.CreateQuadTree();
            m_TileMap.SaveFileTileMap();
        }

        private void mycanvas_MouseLeftButtonDown(object sender, MouseButtonEventArgs e) // event when mouse left pressing
        {
            // if Rectangle checkbox is checked
            if(drawRectangle.IsChecked.Value)
            {
                if(Keyboard.IsKeyDown(Key.LeftCtrl)) // draw rect with Ctrl pressed
                {
                    Point position = e.GetPosition(mycanvas);
                    position.X -= position.X % (tileWidth * ScaleBackGround);
                    position.Y -= position.Y % (tileHeight * ScaleBackGround);

                    double width = Convert.ToDouble(RectWidth.Text.ToString()) * ScaleBackGround;
                    double height = Convert.ToDouble(RectHeight.Text.ToString()) * ScaleBackGround;

                    if (m_ListInfor.FindIndex(t => t == string.Format("ID={0}: {1}", InputID.Text, InputInfor.Text)) == -1)
                        m_ListInfor.Add(string.Format("ID={0}: {1}", InputID.Text, InputInfor.Text));

                    MyRectangle m_Rect = new MyRectangle();

                    m_Rect.rect.Width = width;
                    m_Rect.rect.Height = height;
                    m_Rect.rect.Stroke = Brushes.Black;

                    Canvas.SetLeft(m_Rect.rect, (int)position.X);
                    Canvas.SetTop(m_Rect.rect, (int)position.Y);

                    m_Rect.x = (int)position.X;
                    m_Rect.y = (int)position.Y;
                    m_Rect.id = Convert.ToInt16(InputID.Text.ToString());
                    m_Rect.index = 0;
                    m_ListNode.Add(m_Rect);

                    mycanvas.Children.Add(m_Rect.rect); // draw new rectangle
                }
            }
            else // action for draw image icon
            {
                if (m_ListInfor.FindIndex(t => t == string.Format("ID={0}: {1}", InputID.Text, InputInfor.Text)) == -1)
                    m_ListInfor.Add(string.Format("ID={0}: {1}", InputID.Text, InputInfor.Text));

                MyRectangle rect = new MyRectangle();
                Point position = e.GetPosition(mycanvas);
                position.X -= position.X % (tileWidth * ScaleBackGround);
                position.Y -= position.Y % (tileHeight * ScaleBackGround);

                rect.rect.Width = tileWidth * ScaleBackGround;
                rect.rect.Height = tileHeight * ScaleBackGround;
                rect.rect.Fill = selected_image.Fill;


                mycanvas.Children.Add(rect.rect);
                Canvas.SetLeft(rect.rect, position.X);
                Canvas.SetTop(rect.rect, position.Y);

                rect.id = Convert.ToInt16(InputID.Text.ToString());
                rect.x = (int)position.X;
                rect.y = (int)position.Y;
                rect.index = currentIndex;
                m_ListNode.Add(rect);       
            }
        }

        private void mycanvas_MouseMove(object sender, MouseEventArgs e)
        {
            // update postion of mouse in canvas
            position.Content = string.Format("({0}, {1})", (int)(e.GetPosition(mycanvas).X / ScaleBackGround), (int)(e.GetPosition(mycanvas).Y / ScaleBackGround));
        }


        // remove rectangle draw at position of mouse right click
        private void mycanvas_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point temp = e.GetPosition(mycanvas);

            // remove if right click inside rectangle
            int index = m_ListNode.FindIndex(t => t.x < temp.X && temp.X < t.x + t.rect.Width && t.y < temp.Y && temp.Y < t.y + t.rect.Height);
            if(index != -1)
            {
                // remove rectangle
                mycanvas.Children.Remove(m_ListNode[index].rect);
                m_ListNode.RemoveAt(index);
            }
        }


        //draw grid 
        private void checkBoxDrawGrid_Checked(object sender, RoutedEventArgs e)
        {
            double x = tileWidth * ScaleBackGround;
            double y = tileHeight * ScaleBackGround;
            
            if(checkGrid.IsChecked.Value) // if checked
            { 
                listGrid.Clear();

                for (int i = 0; i < mycanvas.Width / x; i++)
                {
                    Line line = new Line();
                    line.Stroke = System.Windows.Media.Brushes.LightSteelBlue;

                    line.X1 = i * x;
                    line.Y1 = 0;
                    line.X2 = line.X1;
                    line.Y2 = mycanvas.Height;


                    listGrid.Add(line); // store line into list, it will be remove line if Grid not checked
                    mycanvas.Children.Add(line);
                }

                for (int i = 0; i < mycanvas.Height / y; i++)
                {
                    Line line = new Line();
                    line.Stroke = System.Windows.Media.Brushes.LightSteelBlue;

                    line.X1 = 0;
                    line.Y1 = (double)i * y;
                    line.X2 = mycanvas.Width;
                    line.Y2 = line.Y1;


                    listGrid.Add(line);
                    mycanvas.Children.Add(line);
                }
            }
            else
            {
                foreach(var item in listGrid)
                {
                    mycanvas.Children.Remove(item);
                }
            }
        }

        private void exit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void enemy_10_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Enemy_10.Background;
            InputID.Text = "10";
            InputInfor.Text = "enemy mushroom";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void enemy_11_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Enemy_11.Background;
            InputID.Text = "11";
            InputInfor.Text = "enemy flower down";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void enemy_12_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Enemy_12.Background;
            InputID.Text = "12";
            InputInfor.Text = "enemy flower up";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void enemy_13_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Enemy_13.Background;
            InputID.Text = "13";
            InputInfor.Text = "enemy rocket";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void enemy_14_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Enemy_14.Background;
            InputID.Text = "14";
            InputInfor.Text = "enemy turle";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void enemy_15_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Enemy_15.Background;
            InputID.Text = "15";
            InputInfor.Text = "enemy cloud";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void enemy_16_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Enemy_16.Background;
            InputID.Text = "16";
            InputInfor.Text = "enemy boss";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void enemy_17_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Enemy_17.Background;
            InputID.Text = "17";
            InputInfor.Text = "enemy turle fly";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }

        private void item_20_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_20.Background;
            InputID.Text = "20";
            InputInfor.Text = "item mushroom up";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void item_21_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_21.Background;
            InputID.Text = "21";
            InputInfor.Text = "item mushroom bigger";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void item_22_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_22.Background;
            InputID.Text = "22";
            InputInfor.Text = "item mushroom up hidden";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void item_23_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_23.Background;
            InputID.Text = "23";
            InputInfor.Text = "item flower gun";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void item_24_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_24.Background;
            InputID.Text = "24";
            InputInfor.Text = "item coin";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void item_25_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_25.Background;
            InputID.Text = "25";
            InputInfor.Text = "item box";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void item_26_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_26.Background;
            InputID.Text = "26";
            InputInfor.Text = "item move";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void item_27_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_27.Background;
            InputID.Text = "27";
            InputInfor.Text = "item brick";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void item_28_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_28.Background;
            InputID.Text = "28";
            InputInfor.Text = "mushroom up brick";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void item_29_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_29.Background;
            InputID.Text = "29";
            InputInfor.Text = "mushroom bigger brick";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void item_30_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_30.Background;
            InputID.Text = "30";
            InputInfor.Text = "star brick";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void item_31_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_31.Background;
            InputID.Text = "31";
            InputInfor.Text = "gun brick";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void item_32_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_32.Background;
            InputID.Text = "32";
            InputInfor.Text = "coin brick";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }

        private void item_33_Click(object sender, RoutedEventArgs e)
        {
            selected_image.Fill = Item_33.Background;
            InputID.Text = "33";
            InputInfor.Text = "item flag";
            drawRectangle.IsChecked = false;
            currentIndex = 0;
        }
        private void checkBoxDrawRectangle_Checked(object sender, RoutedEventArgs e)
        {
            if(drawRectangle.IsChecked.Value)
            {
                selected_image.Fill = new ImageBrush(rectangle.Source);
                InputID.Text = "1";
                InputInfor.Text = "where mario can stand";

                RectWidth.IsEnabled = true;
                RectHeight.IsEnabled = true;
                currentIndex = 0;
            }
            else
            {
                RectWidth.IsEnabled = false;
                RectHeight.IsEnabled = false;
            }
        }

        private void mycanvasTileMap_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            drawRectangle.IsChecked = false;

            InputID.Text = "0";
            InputInfor.Text = "tiles map";

            Point currentPoint = e.GetPosition(mycanvasTileMap);
            currentPoint.X -= currentPoint.X % (tileWidth);
            currentPoint.Y -= currentPoint.Y % (tileHeight);

            ImageBrush img = new ImageBrush(new BitmapImage( new Uri(m_TextImage)));
            img.Viewbox = new Rect(currentPoint.X / img.ImageSource.Width, currentPoint.Y / img.ImageSource.Height , 16 / img.ImageSource.Width, 16 / img.ImageSource.Height);
            selected_image.Fill = img;

            currentIndex = (int)currentPoint.X / tileWidth + (int)currentPoint.Y / tileHeight * (int)Math.Round(img.ImageSource.Width) / tileWidth;
        }
    }
}
