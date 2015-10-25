using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using System.Xml;


namespace Mapediter
{
    public class RectBit
    {
        Color[,] m_Color;
        int m_Index;
        public bool m_Exist;
        public RectBit(Bitmap _Bitmap, int _Index, int _x, int _y, int _TileWidth, int _TileHeight)
        {
            m_Color = new Color[_TileWidth, _TileHeight];
            m_Index = _Index;

            for(int i = 0; i < _TileWidth; i++)
            {
                for(int j = 0; j < _TileHeight; j++)
                {
                    m_Color[i,j] = _Bitmap.GetPixel(_x + i, _y + j);
                }
            }

            m_Exist = false;
        }

        public Color getColor(int _X, int _Y)
        {
            return m_Color[_X, _Y];
        }
    }


    public class Node
    {
        public int m_Id;
        public int m_Index;
        public int m_X;
        public int m_Y;
        public int m_Width;
        public int m_Height;

        public Node(int _Id, int _Index, int _X, int _Y, int _Width = 0, int _Height = 0)
        {
            m_Id = _Id;
            m_Index = _Index;
            m_X = _X;
            m_Y = _Y;
            m_Width = _Width;
            m_Height = _Height;
        }
    }

    class TileMap
    {
        int m_TileWidth;
        int m_TileHeight;
        int m_MapWidth;
        int m_MapHeight;
        List<Node> m_Temp;
        string m_infor;


        Quadtree m_QuadTree;
        public TileMap(List<Node> _List , string _Information,int _TileWidth, int _TileHeight, int mapWidth, int mapHeight)
        {
            m_TileHeight = _TileHeight;
            m_TileWidth = _TileWidth;
            m_Temp = _List;
            m_infor = _Information;
            m_MapWidth = mapWidth;
            m_MapHeight = mapHeight;
        }

        


        public void CreateQuadTree()
        {
            // make real world left = x, right = x + width, top = y + height, bottom = y
            foreach (var item in m_Temp)
                item.m_Y += item.m_Height;

            m_QuadTree = new Quadtree(0, 0, m_MapWidth, MainWindow.minWidth);
            foreach(var item in m_Temp)
            {
                item.m_Y = m_MapHeight - item.m_Y;
                m_QuadTree.Insert(item);
            }
        }

        void SavesNode(List<Node> list, XmlWriter write)
        {
            foreach (var item in list)
            {
                write.WriteStartElement("Object");

                write.WriteAttributeString("id", item.m_Id.ToString());
                write.WriteAttributeString("index", item.m_Index.ToString());
                write.WriteAttributeString("x", item.m_X.ToString());
                write.WriteAttributeString("y", item.m_Y.ToString());
                write.WriteAttributeString("width", item.m_Width.ToString());
                write.WriteAttributeString("height", item.m_Height.ToString());

                write.WriteEndElement();
            }
        }

        void SavesNode(Quadtree quadTree, string idNode, XmlWriter write)
        {
            if (quadTree == null) return;

            if(quadTree.m_List.Count != 0)
            {
                write.WriteStartElement("Node");
                write.WriteAttributeString("id", idNode);
                foreach(var item in quadTree.m_List)
                {
                    write.WriteStartElement("Object");

                    write.WriteAttributeString("id", item.m_Id.ToString());
                    write.WriteAttributeString("index", item.m_Index.ToString());
                    write.WriteAttributeString("x", item.m_X.ToString());
                    write.WriteAttributeString("y", item.m_Y.ToString());
                    write.WriteAttributeString("width", item.m_Width.ToString());
                    write.WriteAttributeString("height", item.m_Height.ToString());
                    
                    write.WriteEndElement();
                }
                write.WriteEndElement();
            }
            else
            {
                double id = int.Parse(idNode) * 10;

                if(quadTree.m_AreaOne != null)
                {
                    SavesNode(quadTree.m_AreaOne, (id + 1).ToString(), write);
                }

                if (quadTree.m_AreaTwo != null)
                {
                    SavesNode(quadTree.m_AreaTwo, (id + 2).ToString(), write);
                }

                if (quadTree.m_AreaThree != null)
                {
                    SavesNode(quadTree.m_AreaThree, (id + 3).ToString(), write);
                }

                if (quadTree.m_AreaFour != null)
                {
                    SavesNode(quadTree.m_AreaFour, (id + 4).ToString(), write);
                }
            }
        }

        public void SaveFileTileMap()
        {
            SaveFileDialog saveFile = new SaveFileDialog();
            saveFile.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
            saveFile.FileName = "TileNode.xml";

            if (saveFile.ShowDialog() == DialogResult.OK)
            {
                string path = saveFile.FileName;
                DirectoryInfo direc = new DirectoryInfo(path);
                path = direc.Parent.FullName;
                try
                {
                        
                    // new code 14/10
                    // write quadtree into xml file
                    XmlWriterSettings xmlSetting = new XmlWriterSettings();
                    xmlSetting.Indent = true;
                    xmlSetting.IndentChars = "\t";
                    XmlWriter xmlWriter = XmlWriter.Create(path + "\\TileNode.xml", xmlSetting);
                   
                    xmlWriter.WriteStartDocument();


                    xmlWriter.WriteStartElement("Game");
                    
                    // information
                    xmlWriter.WriteStartElement("Information");
                    string[] split = m_infor.Split('\n');
                    foreach(string item in split)
                    {
                        xmlWriter.WriteStartElement("Description");
                        xmlWriter.WriteAttributeString("des", item);
                        xmlWriter.WriteEndElement();
                    }
                    xmlWriter.WriteEndElement();
                    
                    
                    // save quadtree
                    xmlWriter.WriteStartElement("Quadtree");
                    xmlWriter.WriteAttributeString("mapwidth", m_QuadTree.m_Rect.width.ToString());
                    xmlWriter.WriteAttributeString("minwidth", m_QuadTree.minWidth.ToString());

                    SavesNode(m_QuadTree.m_AreaOne, "1", xmlWriter);
                    SavesNode(m_QuadTree.m_AreaTwo, "2", xmlWriter);
                    SavesNode(m_QuadTree.m_AreaThree, "3", xmlWriter);
                    SavesNode(m_QuadTree.m_AreaFour, "4", xmlWriter);

                    xmlWriter.WriteEndElement();
                    xmlWriter.WriteEndElement();
                    
                    xmlWriter.WriteEndDocument();
                    xmlWriter.Close();
                    
                   
                    MessageBox.Show("Xuat file thanh cong.");
                }
                catch(Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }
    }
}
