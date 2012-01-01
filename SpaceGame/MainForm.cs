using System;
using System.Collections.Generic; 
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SpaceGame {     
    
    public partial class MainForm : Form {         
        
        private Bitmap _bmpPrimarySurface = null;         
        private GameCore.GameClass _GameClass = null;         
        private Timer _Timer1 = null;         
        private Timer _Timer2 = null;          
        
        public MainForm() {             
            
            InitializeComponent();             
            _InitializeSystem();         
        
        }          
        
        void _InitializeSystem() {  
     
            /////////////////////////////////////////////////             
            // Load game resources (commented out for now)             
            GameCore.ResourceLoader._LoadResources();             
            /////////////////////////////////////////////////
              
            _bmpPrimarySurface = new Bitmap(_pictureBox.Width, _pictureBox.Height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);                          
            _GameClass = new GameCore.GameClass();             
            _GameClass._OnInitialize(_pictureBox.Width, _pictureBox.Height);              
            _Timer1 = new Timer();             
            _Timer1.Tick += new EventHandler(_OnDrawTimer);             
            _Timer1.Interval = 25;             
            _Timer1.Start();              
            _Timer2 = new Timer();             
            _Timer2.Tick += new EventHandler(_OnRunAITimer);             
            _Timer2.Interval = 35;             
            _Timer2.Start();         
        
        }          
        
        void _OnDrawTimer(object sender, EventArgs e) {             
            
            lock (_bmpPrimarySurface) {                 
                
                _OnDraw(_bmpPrimarySurface);             
            
            }         
        
        }          
        
        void _OnRunAITimer(object sender, EventArgs e) {             
            
            lock (_bmpPrimarySurface) {                 
                
                _OnRunAI();             
            
            }         
        
        }          
        
        private void _OnRunAI() {             
            
            _GameClass._OnRunAI();         
        
        }          
        
        private void _OnDraw(Bitmap bmp) {             
            
            Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);              
            System.Drawing.Imaging.BitmapData bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bmp.PixelFormat);             
            IntPtr ptr = bmpData.Scan0;              
            _GameClass._OnDraw(bmp.Width, bmp.Height, bmpData.Stride, ptr);              
            bmp.UnlockBits(bmpData);              
            _pictureBox.Image = _bmpPrimarySurface;         
        
        }     
    
    } 

}
