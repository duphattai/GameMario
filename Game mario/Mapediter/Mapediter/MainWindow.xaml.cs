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

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 
    public partial class MainWindow : Window
    {
        int ScaleBackGround;
        int tileWidth, tileHeight;
        string m_TextImage;
        TileMap m_TileMap = null;

        string m_Information = "ID=0: tile map" + Environment.NewLine;

        List<string> m_ListInfor = new List<string>();
       
        List<Node> m_ListNode = new List<Node>();// store position of rectangle draw on canvas
        List<Rectangle> m_ListRectangle = new List<Rectangle>(); // store rectangle it draw on canvas
        Rectangle m_Rect = null;

        List<Line> listGrid = new List<Line>(); // store lines will draw on canvas 

        List<Node> m_ListNodeImage = new List<Node>(); // store position of icon draw on canvas
        List<Image> listIcon = new List<Image>(); // store icon will draw on canvas

        public static int minWidth; // use for quadtree

        // draw a rectangle create from startPoint and endPoint
        Point startPoint;
        Point endPoint;

        bool isMouseMove_Pressed = false;
        public MainWindow()
        {
            InitializeComponent();
            mycanvas.Focus();
            Keyboard.Focus(mycanvas);
        }
  
        private void open_Click(object sender, RoutedEventArgs e)
        {
            OpenFile form = new OpenFile();
            form.ShowDialog();

            ScaleBackGround = (int)form.scale;
            m_TextImage = form.fileImagePath;
            tileHeight = form.tileHeight;
            tileWidth = form.tileWidth;
            minWidth = form.minWidth;

            ImageBrush img = new ImageBrush();
            img.ImageSource = new BitmapImage(new Uri(m_TextImage));

            mycanvas.Width = img.ImageSource.Width * ScaleBackGround;
            mycanvas.Height = img.ImageSource.Height * ScaleBackGround;
            mycanvas.Background = img;
        }

        private void createTiledMap_Click(object sender, RoutedEventArgs e)
        {
            foreach(var item in m_ListNode)
            {
                item.m_X /= ScaleBackGround;
                item.m_Y /= ScaleBackGround;
                item.m_Width /= ScaleBackGround;
                item.m_Height /= ScaleBackGround;
            }

            foreach(var item in m_ListInfor)
            {
                m_Information += item + Environment.NewLine;
            }
            //m_Information += "ID\tIndex\tX\tY\tWidth\tHeight" + Environment.NewLine;

            m_ListNode.AddRange(m_ListNodeImage);
            m_TileMap = new TileMap(m_TextImage, m_ListNode, m_Information, tileWidth, tileHeight);
            m_TileMap.CreateRectBit();
            m_TileMap.CreateTileMap();
            m_TileMap.CreateQuadTree();
            m_TileMap.SaveFileTileMap();
        }

        private void mycanvas_MouseLeftButtonDown(object sender, MouseButtonEventArgs e) // event when mouse left pressing
        {
            // if Rectangle checkbox is checked
            if(drawRectangle.IsChecked.Value)
            {
                m_Rect = new Rectangle();
                if(Keyboard.IsKeyDown(Key.LeftCtrl)) // draw rect with Ctrl pressed
                {
                    Point position = e.GetPosition(mycanvas);
                    position.X -= position.X % (tileWidth * ScaleBackGround);
                    position.Y -= position.Y % (tileHeight * ScaleBackGround);

                    double width = Convert.ToDouble(RectWidth.Text.ToString()) * ScaleBackGround;
                    double height = Convert.ToDouble(RectHeight.Text.ToString()) * ScaleBackGround;

                    if (m_ListInfor.FindIndex(t => t == string.Format("ID={0}: {1}", InputID.Text, InputInfor.Text)) == -1)
                        m_ListInfor.Add(string.Format("ID={0}: {1}", InputID.Text, InputInfor.Text));

                    m_Rect.Width = width;
                    m_Rect.Height = height;
                    m_Rect.Stroke = Brushes.Black;
                    Canvas.SetLeft(m_Rect, (int)position.X);
                    Canvas.SetTop(m_Rect, (int)position.Y);

                    m_ListRectangle.Add(m_Rect);

                    Node node = new Node(Convert.ToInt16(InputID.Text.ToString()), 0, (int)position.X, (int)position.Y, (int)width, (int)height);
                    m_ListNode.Add(node);

                    mycanvas.Children.Add(m_Rect); // draw new rectangle
                }
                else
                {
                    startPoint = e.GetPosition(mycanvas);
                   
                    m_Rect.Width = 0;
                    m_Rect.Height = 0;
                    m_Rect.Stroke = Brushes.Black;
                    Canvas.SetLeft(m_Rect, startPoint.X);
                    Canvas.SetTop(m_Rect, startPoint.Y);

                    mycanvas.Children.Add(m_Rect);
                }    
            }
            else // action for draw image icon
            {
                if(m_ListInfor.FindIndex(t => t == string.Format("ID={0}: {1}", InputID.Text, InputInfor.Text)) == -1)
                    m_ListInfor.Add(string.Format("ID={0}: {1}", InputID.Text, InputInfor.Text));
                

                Image img = new Image();
                img.Source = image_Selected.Source;
                img.LayoutTransform = new ScaleTransform(ScaleBackGround, ScaleBackGround);
                
               
                Point position = e.GetPosition(mycanvas);
                position.X -= position.X % (tileWidth * ScaleBackGround);
                position.Y -= position.Y % (tileHeight * ScaleBackGround);

                mycanvas.Children.Add(img);
                Canvas.SetLeft(img, position.X);
                Canvas.SetTop(img, position.Y);

                // make not draw icons at the same position
                Node temp = m_ListNodeImage.Find(t => t.m_X == position.X && t.m_Y == position.Y);
                if(temp == null)
                    m_ListNodeImage.Add(new Node(Convert.ToInt16(InputID.Text.ToString()), 0, (int)(position.X), (int)(position.Y), tileWidth * ScaleBackGround, tileHeight * ScaleBackGround));
                else
                    temp.m_Id = Convert.ToInt16(InputID.Text.ToString());

                listIcon.Add(img);
            }
        }

        private void mycanvas_MouseMove(object sender, MouseEventArgs e)
        {
            isMouseMove_Pressed = false;
            // update postion of mouse in canvas
            position.Content = string.Format("({0}, {1})", (int)(e.GetPosition(mycanvas).X / ScaleBackGround), (int)(e.GetPosition(mycanvas).Y / ScaleBackGround));

            if (e.LeftButton == MouseButtonState.Pressed && drawRectangle.IsChecked.Value)
            {
                mycanvas.Children.Remove(m_Rect); // clear old rectangle

                endPoint = e.GetPosition(mycanvas);

                int width = (int)Math.Abs(startPoint.X - endPoint.X);
                int height = (int)Math.Abs(startPoint.Y - endPoint.Y);

                m_Rect.Width = width;
                m_Rect.Height = height;
                m_Rect.Stroke = Brushes.Black;

                if (endPoint.X > startPoint.X)
                    Canvas.SetLeft(m_Rect, (int)startPoint.X);
                else
                    Canvas.SetLeft(m_Rect, (int)endPoint.X);


                if (endPoint.Y < startPoint.Y)
                    Canvas.SetTop(m_Rect, (int)endPoint.Y);
                else
                    Canvas.SetTop(m_Rect, (int)startPoint.Y);

                mycanvas.Children.Add(m_Rect); // update new rectangle

                if (width != 0 || height != 0)
                    isMouseMove_Pressed = true;
            }
        }

        // event when mouse left release, add rectangle, add node
        private void mycanvas_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if(isMouseMove_Pressed)
            {
                if (m_ListInfor.FindIndex(t => t == string.Format("ID={0}: {1}", InputID.Text, InputInfor.Text)) == -1)
                    m_ListInfor.Add(string.Format("ID={0}: {1}", InputID.Text, InputInfor.Text));


                m_ListRectangle.Add(m_Rect);
                
                Node node = new Node(Convert.ToInt16(InputID.Text.ToString()), 0, 0, 0);

                if(endPoint.X < startPoint.X)
                    node.m_X = (int) (endPoint.X);
                else 
                    node.m_X = (int)(startPoint.X);
                
                if (endPoint.Y < startPoint.Y)
                    node.m_Y = (int)(endPoint.Y);
                else
                    node.m_Y = (int)(startPoint.Y);

                node.m_Width = (int)Math.Abs((endPoint.X - startPoint.X));
                node.m_Height = (int)Math.Abs((endPoint.Y - startPoint.Y));

                m_ListNode.Add(node);
            }

            isMouseMove_Pressed = false;
        } 

        // remove rectangle draw at position of mouse right click
        private void mycanvas_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point temp = e.GetPosition(mycanvas);

            // remove if right click inside rectangle
            int index = m_ListNode.FindIndex(t => t.m_X < temp.X && temp.X < t.m_X + t.m_Width && t.m_Y < temp.Y && temp.Y < t.m_Y + t.m_Height);
            if(index != -1)
            {
                // remove rectangle
                mycanvas.Children.Remove(m_ListRectangle[index]);
                m_ListRectangle.RemoveAt(index);
                m_ListNode.RemoveAt(index);
            }

            // remove if right click inside icon 
            index = m_ListNodeImage.FindIndex(t => t.m_X < temp.X && temp.X < t.m_X + t.m_Width && t.m_Y < temp.Y && temp.Y < t.m_Y + t.m_Height);
            if(index != -1)// exist
            {
                mycanvas.Children.Remove(listIcon[index]);
                m_ListNodeImage.RemoveAt(index);
                listIcon.RemoveAt(index);
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
            image_Selected.Source = new BitmapImage(new Uri(@"\Enemies\10.png", UriKind.Relative));
            image_Selected.Tag = @"\Enemies\10.png";
            InputID.Text = "10";
            InputInfor.Text = "enemy mushroom";
            drawRectangle.IsChecked = false;
        }

        private void enemy_11_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Enemies\11.png", UriKind.Relative));
            image_Selected.Tag = @"\Enemies\11.png";
            InputID.Text = "11";
            InputInfor.Text = "enemy mushroom blue";
            drawRectangle.IsChecked = false;
        }

        private void enemy_12_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Enemies\12.png", UriKind.Relative));
            image_Selected.Tag = @"\Enemies\12.png";
            InputID.Text = "12";
            InputInfor.Text = "enemy flower";
            drawRectangle.IsChecked = false;
        }

        private void enemy_13_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Enemies\13.png", UriKind.Relative));
            image_Selected.Tag = @"\Enemies\13.png";
            InputID.Text = "13";
            InputInfor.Text = "enemy flower blue";
            drawRectangle.IsChecked = false;
        }

        private void enemy_14_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Enemies\14.png", UriKind.Relative));
            image_Selected.Tag = @"\Enemies\14.png";
            InputID.Text = "14";
            InputInfor.Text = "enemy turle";
            drawRectangle.IsChecked = false;
        }

        private void enemy_15_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Enemies\15.png", UriKind.Relative));
            image_Selected.Tag = @"\Enemies\15.png";
            InputID.Text = "15";
            InputInfor.Text = "enemy turle red";
            drawRectangle.IsChecked = false;
        }
        private void enemy_16_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Enemies\16.png", UriKind.Relative));
            image_Selected.Tag = @"\Enemies\16.png";
            InputID.Text = "16";
            InputInfor.Text = "enemy turle blue";
            drawRectangle.IsChecked = false;
        }
        private void enemy_17_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Enemies\17.png", UriKind.Relative));
            image_Selected.Tag = @"\Enemies\17.png";
            InputID.Text = "17";
            InputInfor.Text = "enemy turle fly";
            drawRectangle.IsChecked = false;
        }

        private void enemy_18_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Enemies\18.png", UriKind.Relative));
            image_Selected.Tag = @"\Enemies\18.png";
            InputID.Text = "18";
            InputInfor.Text = "enemy turle fly red";
            drawRectangle.IsChecked = false;
        }
        private void item_20_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Items\20.png", UriKind.Relative));
            image_Selected.Tag = @"\Items\20.png";
            InputID.Text = "20";
            InputInfor.Text = "item mushroom bigger";
            drawRectangle.IsChecked = false;
        }
        private void item_21_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Items\21.png", UriKind.Relative));
            image_Selected.Tag = @"\Items\21.png";
            InputID.Text = "21";
            InputInfor.Text = "item mushroom bigger";
            drawRectangle.IsChecked = false;
        }
        private void item_22_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Items\22.png", UriKind.Relative));
            image_Selected.Tag = @"\Items\22.png";
            InputID.Text = "22";
            InputInfor.Text = "item star";
            drawRectangle.IsChecked = false;
        }
        private void item_23_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Items\23.png", UriKind.Relative));
            image_Selected.Tag = @"\Items\23.png";
            InputID.Text = "23";
            InputInfor.Text = "item flower gun";
            drawRectangle.IsChecked = false;
        }
        private void item_24_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Items\24.png", UriKind.Relative));
            image_Selected.Tag = @"\Items\24.png";
            InputID.Text = "24";
            InputInfor.Text = "item coin";
            drawRectangle.IsChecked = false;
        }
        private void item_25_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Items\25.png", UriKind.Relative));
            image_Selected.Tag = @"\Items\25.png";
            InputID.Text = "25";
            InputInfor.Text = "item box";
            drawRectangle.IsChecked = false;
        }

        private void item_26_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Items\26.png", UriKind.Relative));
            image_Selected.Tag = @"\Items\26.png";
            InputID.Text = "26";
            InputInfor.Text = "item move";
            drawRectangle.IsChecked = false;
        }

        private void item_27_Click(object sender, RoutedEventArgs e)
        {
            image_Selected.Source = new BitmapImage(new Uri(@"\Items\27.png", UriKind.Relative));
            image_Selected.Tag = @"\Items\27.png";
            InputID.Text = "27";
            InputInfor.Text = "item brick";
            drawRectangle.IsChecked = false;
        }


        private void checkBoxDrawRectangle_Checked(object sender, RoutedEventArgs e)
        {
            if(drawRectangle.IsChecked.Value)
            {
                image_Selected.Source = new BitmapImage(new Uri(@"\Items\rectangle.png", UriKind.Relative));
                InputID.Text = "1";
                InputInfor.Text = "where mario can stand";

                RectWidth.IsEnabled = true;
                RectHeight.IsEnabled = true;
            }
            else
            {
                RectWidth.IsEnabled = false;
                RectHeight.IsEnabled = false;
            }
        }

    }
}
