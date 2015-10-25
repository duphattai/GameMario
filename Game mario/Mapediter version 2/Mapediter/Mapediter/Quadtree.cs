using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mapediter
{
    class Box
    {
        public double x;
        public double y;
        public double width;
        public double height;

        public Box(double x, double y, double w, double h)
        {
            this.x = x;
            this.y = y;
            width = w;
            height = h;
        }
    }


    class Quadtree
    {
        public double minWidth;
        
        public Box m_Rect;
        public List<Node> m_List;


        public Quadtree m_AreaOne; // x > 0, y > 0
        public Quadtree m_AreaTwo; // x < 0, y > 0
        public Quadtree m_AreaThree; // x < 0, y < 0
        public Quadtree m_AreaFour;  // x > 0, y < 0

        public Quadtree(double x, double y, double width, double minWidth)
        {
            this.minWidth = minWidth;
            m_Rect = new Box(x, y, width, width);
            m_List = new List<Node>();

            m_AreaOne = null;
            m_AreaTwo = null;
            m_AreaThree = null;
            m_AreaFour = null;
        }

        void Split()
        {
            if(m_AreaOne == null)
                m_AreaOne = new Quadtree(m_Rect.x + m_Rect.width / 2, m_Rect.y + m_Rect.height / 2, m_Rect.width / 2, minWidth);
            
            if(m_AreaTwo == null)
                m_AreaTwo = new Quadtree(m_Rect.x, m_Rect.y + m_Rect.height / 2, m_Rect.width / 2, minWidth);
            
            if(m_AreaThree == null)
                m_AreaThree = new Quadtree(m_Rect.x, m_Rect.y, m_Rect.width / 2, minWidth);

            if (m_AreaFour == null)
                m_AreaFour = new Quadtree(m_Rect.x + m_Rect.width / 2, m_Rect.y, m_Rect.width / 2, minWidth);
        }

        bool isContain(Node node)
        {
            return !(node.m_X + node.m_Width < m_Rect.x || node.m_X > m_Rect.x + m_Rect.width ||
                node.m_Y + node.m_Height < m_Rect.y || node.m_Y > m_Rect.y + m_Rect.height);
        }

        public void Insert(Node node)
        {
            if(m_Rect.width > minWidth) // đệ quy lần nữa
            {
                Split();

                if (m_AreaOne.isContain(node))
                    m_AreaOne.Insert(node);

                if (m_AreaTwo.isContain(node))
                    m_AreaTwo.Insert(node);

                if (m_AreaThree.isContain(node))
                    m_AreaThree.Insert(node);

                if (m_AreaFour.isContain(node))
                    m_AreaFour.Insert(node);
            }
            else 
            {
                m_List.Add(node);
            }
        }
    }
}
