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
        Bitmap m_Bitmap;
        int m_TileWidth;
        int m_TileHeight;
        List<RectBit> m_ArrayRectBit;
        List<Node> m_Temp;
        List<Node> m_ArrayNode;
        string m_infor;


        Quadtree m_QuadTree;
        public TileMap(string _Path, List<Node> _List , string _Information,int _TileWidth, int _TileHeight)
        {
            m_Bitmap = new Bitmap(_Path);
            m_TileHeight = _TileHeight;
            m_TileWidth = _TileWidth;
            m_Temp = _List;
            m_infor = _Information;
        }

        public void CreateRectBit()
        {
            m_ArrayRectBit = new List<RectBit>();
            m_ArrayNode = new List<Node>();
            int n = 0;

            for (int j = 0; j < m_Bitmap.Height / m_TileHeight; j++)
            {
                for(int i = 0; i < m_Bitmap.Width / m_TileWidth; i++)
                {
                    m_ArrayRectBit.Add(new RectBit(m_Bitmap, n, i * m_TileWidth, j *m_TileHeight, m_TileWidth, m_TileHeight));
                    m_ArrayNode.Add(new Node(0, n, i * m_TileWidth, j * m_TileHeight, m_TileWidth, m_TileHeight));
                    n++;
                }
            }
        }

        bool CompareTwoRectBit(RectBit _Rect1, RectBit _Rect2)
        {
            for(int i = 0; i < m_TileWidth; i++)
            {
                for(int j = 0; j < m_TileHeight; j++)
                {
                    if (_Rect1.getColor(i, j) != _Rect2.getColor(i, j)) 
                        return false;
                }
            }

            return true;
        }

        public void CreateTileMap()
        {
            int n = -1;
            for(int i = 0; i < m_ArrayRectBit.Count; i++)
            {
               
                if (!m_ArrayRectBit[i].m_Exist) n++;
                else continue;
                for(int j = i + 1; j < m_ArrayRectBit.Count; j++)
                {
                    if( CompareTwoRectBit(m_ArrayRectBit[i], m_ArrayRectBit[j]))
                    {
                        m_ArrayNode[j].m_Index = n;
                        m_ArrayRectBit[j].m_Exist = true;
                    }
                }

                m_ArrayNode[i].m_Index = n;
            }

            for (int i = 0; i < m_ArrayRectBit.Count; i++)
            {
                if(m_ArrayRectBit[i].m_Exist)
                {
                    m_ArrayRectBit.RemoveAt(i);
                    i--;
                }
            }
        }

        public void CreateQuadTree()
        {
            // make real world left = x, right = x + width, top = y + height, bottom = y
            foreach (var item in m_Temp)
                item.m_Y += item.m_Height;

            //for (int i = 0; i < m_ArrayNode.Count; i++)
            //{
            //    m_ArrayNode[i].m_Y += m_ArrayNode[i].m_Height;
            //    m_Temp.Add(m_ArrayNode[i]);
            //}

            m_QuadTree = new Quadtree(0, 0, m_Bitmap.Width, MainWindow.minWidth);
            foreach(var item in m_Temp)
            {
                item.m_Y = m_Bitmap.Height - item.m_Y;
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
                    
                    //Tài: 14/10
                    /*
                    // make real world left = x, right = x + width, top = y + height, bottom = y
                    foreach (var item in m_Temp)
                        item.m_Y += item.m_Height;

                    for (int i = 0; i < m_ArrayNode.Count; i++)
                    {
                        m_ArrayNode[i].m_Y += m_ArrayNode[i].m_Height;
                        m_Temp.Add(m_ArrayNode[i]);
                    }
                        

                    using (StreamWriter file = new StreamWriter(path +"\\TileNode.txt"))
                    {
                        file.Write(m_infor);
                        string temp;
                        for (int i = 0; i < m_Temp.Count; i++)
                        {
                            temp = null;
                            temp = m_Temp[i].m_Id + "\t" + m_Temp[i].m_Index + "\t" + m_Temp[i].m_X + "\t" + Convert.ToString(m_Bitmap.Height - m_Temp[i].m_Y) + "\t" + m_Temp[i].m_Width + "\t" + m_Temp[i].m_Height;
                            file.WriteLine(temp.ToString());
                        }
                    }
                    */


                    for (int i = 0; i < m_ArrayNode.Count; i++)
                    {
                        m_ArrayNode[i].m_Y += m_ArrayNode[i].m_Height;
                        m_ArrayNode[i].m_Y = m_Bitmap.Height - m_ArrayNode[i].m_Y;
                    }
                        
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
                    //save tile map
                    xmlWriter.WriteStartElement("Tilemap");
                    SavesNode(m_ArrayNode, xmlWriter);
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
                    
                    
                    //draw tileMap
                    Bitmap bitmap = new Bitmap(m_ArrayRectBit.Count * m_TileWidth, m_TileHeight);

                    for (int k = 0; k < m_ArrayRectBit.Count; k++)
                    {
                        for (int j = 0; j < m_TileHeight; j++)
                        {
                            for (int i = 0; i < m_TileWidth; i++)
                            {
                                bitmap.SetPixel(k * m_TileWidth + i, j, m_ArrayRectBit[k].getColor(i, j));
                            }
                        }
                    }

                    bitmap.Save(path + "\\TileMap.png");
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
