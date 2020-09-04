using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Data.SQLite;
using System.IO;

namespace WindowsFormsApp1
{
   
    public partial class YLJtools : Form
    {


        public static SerialPort _serialPort;
        public static bool serial_status = false;

        public YLJtools()
        {
            InitializeComponent();

        }

        private void YLJtools_Load(object sender, EventArgs e)
        {

            foreach (string s in SerialPort.GetPortNames())
            {
                Console.WriteLine("   {0}", s);
                cbx_comx.Items.Add(s);
            }
            cbx_comx.SelectedIndex = 0;
            foreach (string s in Enum.GetNames(typeof(Parity)))
            {
                Console.WriteLine("   {0}", s);
                cbx_paritybit.Items.Add(s);
            }
            cbx_paritybit.SelectedIndex = 0;
            foreach (string s in Enum.GetNames(typeof(StopBits)))
            {
                Console.WriteLine("   {0}", s);
                cbx_stopbit.Items.Add(s);
            }
            cbx_stopbit.SelectedIndex = 1;

            tbx_baud.Text = "115200";
            tbx_databit.Text = "8";

            if (serial_status)
            {
                btn_serial_switch.Text = "Close";
                
            }
            else
            {
                btn_serial_switch.Text = "Open";
            }


            check_sqlite();

        }

        private void btn_serial_switch_Click(object sender, EventArgs e)
        {

            string parity;
            string stopBits;
           
           


            if (serial_status == false)
            {

                 _serialPort = new SerialPort();
                parity = cbx_paritybit.SelectedItem.ToString();
                stopBits = cbx_stopbit.SelectedItem.ToString();
                //Console.WriteLine("_serialPort.PortName : %s", _serialPort.PortName.ToString());
                string test_str;
                test_str = _serialPort.PortName.ToString();
                Console.Write("%s", test_str);
                _serialPort.PortName = cbx_comx.SelectedItem.ToString();
                _serialPort.BaudRate = Convert.ToInt32( tbx_baud.Text.ToString());
                _serialPort.Parity = (Parity)Enum.Parse(typeof(Parity), parity, true);
                _serialPort.DataBits = Convert.ToInt32(tbx_databit.Text.ToString());
                _serialPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), stopBits, true);
                //_serialPort.Handshake = ;

                // Set the read/write timeouts
                _serialPort.ReadTimeout = 500;
                _serialPort.WriteTimeout = 500;
                serial_status = true;
                btn_serial_switch.Text = "Close";
                _serialPort.Open();

                _serialPort.DataReceived += new SerialDataReceivedEventHandler(_serialPort_DataReceived);



            }
            else if(serial_status == true)
            {
                serial_status = false;
                btn_serial_switch.Text = "Open";
                _serialPort.Close();
            

            }
            
        }

        private void _serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            System.Threading.Thread.Sleep(100);
            this.Invoke((EventHandler)(delegate
            {
                Byte[] revdatabuf = new byte[_serialPort.BytesToRead];
                _serialPort.Read(revdatabuf, 0, revdatabuf.Length);
                if (revdatabuf.Length > 0)
                {
                    rev_process(revdatabuf, (UInt16)revdatabuf.Length);
                }
                

                /*
                for (int i = 0; i < revdatabuf.Length; i++)
                {
                    tbx_revdata.Text += revdatabuf[i].ToString("X2") + " ";
                }
                tbx_revdata.Text += "\r\n";
                */
                _serialPort.DiscardInBuffer();
            }
                ));
        }

        UInt32 rev_count = 0;

        public byte rev_calc_checksum(byte[] buf, UInt16 len)
        {
            byte sum = 0;
            for (uint i = 1; i < len - 3; i++)
            {
                sum += buf[i];
            }
            return sum;
        }

        TimeSpan ts_temp;
        public void rev_process(Byte[] buf,UInt16 len)
        {
            if (buf[0] == 0x7E)
            {
                if (buf[1] == 0x42)
                {
                    if (len == (buf[2] + buf[3] * 256 + 6))
                    {
                        if (rev_calc_checksum(buf, len) == buf[len - 2])
                        {
                            rev_count++;
                            float acc_peak;
                            float base_frequency;
                            float harmonic_peak_0_5;
                            float harmonic_peak_1;
                            float harmonic_peak_1_5;
                            float harmonic_peak_2;
                            float harmonic_peak_2_5;
                            float harmonic_peak_3;
                            float harmonic_peak_4;
                            float harmonic_peak_5;
                            float harmonic_peak_6;
                            float speed_peak;
                            float offset_peak;
                            float temperature;
                            byte power;
                            float acc_mean;
                            float acc_min;
                            float displace_min;

                            acc_peak = BitConverter.ToSingle(buf, 4);
                            base_frequency = BitConverter.ToSingle(buf, 4 * 2);
                            harmonic_peak_0_5 = BitConverter.ToSingle(buf, 4 * 3);
                            harmonic_peak_1 = BitConverter.ToSingle(buf, 4 * 4);
                            harmonic_peak_1_5 = BitConverter.ToSingle(buf, 4 * 5);
                            harmonic_peak_2 = BitConverter.ToSingle(buf, 4 * 6);
                            harmonic_peak_2_5 = BitConverter.ToSingle(buf, 4 * 7);
                            harmonic_peak_3 = BitConverter.ToSingle(buf, 4 * 8);
                            harmonic_peak_4 = BitConverter.ToSingle(buf, 4 * 9);
                            harmonic_peak_5 = BitConverter.ToSingle(buf, 4 * 10);
                            harmonic_peak_6 = BitConverter.ToSingle(buf, 4 * 11);
                            speed_peak = BitConverter.ToSingle(buf, 4 * 12);
                            offset_peak = BitConverter.ToSingle(buf, 4 * 13);
                            temperature = BitConverter.ToSingle(buf, 4 * 14);
                            power = buf[4 * 15];  //BitConverter.ToSingle(buf, 4*15);
                            acc_mean = BitConverter.ToSingle(buf, 4 * 16 - 3);
                            acc_min = BitConverter.ToSingle(buf, 4 * 17 - 3);
                            displace_min = BitConverter.ToSingle(buf, 4 * 18 - 3);

                            string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory);
                            //string path = "E://holiday.json";
                           // path = Path.Combine(path + "\\holiday.json");

                            string sql_path = "data source=" + path + "\\sql.db";
                            using (SQLiteConnection conn = new SQLiteConnection(sql_path))
                            {
                                using (SQLiteCommand cmd = new SQLiteCommand())
                                {
                                    cmd.Connection = conn;
                                    conn.Open();

                                    SQLiteHelper sh = new SQLiteHelper(cmd);

                                    // do something...
                                    DateTime.Now.ToString("hh:mm:ss");

                                    TimeSpan ts = DateTime.Now - new DateTime(1970, 1, 1, 0, 0, 0, 0);

                                    var dic = new Dictionary<string, object>();
                                    dic["time"] = DateTime.Now.ToString("hh:mm:ss.fff");
                                    dic["acc_uppeak"] = acc_peak.ToString();
                                    dic["acc_downpeak"] = acc_min.ToString();
                                    dic["acc_mean"] = acc_mean.ToString();
                                    dic["acc_05"] = harmonic_peak_0_5.ToString();
                                    dic["acc_1"] = harmonic_peak_1.ToString();
                                    dic["acc_15"] = harmonic_peak_1_5.ToString();
                                    dic["speedRMS"] = speed_peak.ToString();
                                    dic["dis_uppeak"] = offset_peak.ToString();
                                    dic["dis_downpeak"] = displace_min.ToString();
                                    dic["base_freq"] = base_frequency.ToString();
                                    dic["time_dif"] = (ts - ts_temp).ToString();
                                    sh.Insert("normal_data", dic);

/*
                                    SQLiteTable tb = new SQLiteTable("normal_data");
                                    tb.Columns.Add(new SQLiteColumn("id", true));
                                    tb.Columns.Add(new SQLiteColumn("time"));
                                    tb.Columns.Add(new SQLiteColumn("acc_max"));
                                    tb.Columns.Add(new SQLiteColumn("acc_min"));
                                    tb.Columns.Add(new SQLiteColumn("acc_mean"));
                                    tb.Columns.Add(new SQLiteColumn("acc_05"));
                                    tb.Columns.Add(new SQLiteColumn("acc_1"));
                                    tb.Columns.Add(new SQLiteColumn("acc_15"));
                                    tb.Columns.Add(new SQLiteColumn("base_freq"));
                                    tb.Columns.Add(new SQLiteColumn("speedRMS"));
                                    tb.Columns.Add(new SQLiteColumn("dis_max"));
                                    tb.Columns.Add(new SQLiteColumn("dis_min"));
                                    tb.Columns.Add(new SQLiteColumn("time_dif"));
*/



                                    conn.Close();
                                }
                            }

                            ts_temp = DateTime.Now - new DateTime(1970, 1, 1, 0, 0, 0, 0);



                            lvw_DataValue.Items.Clear();
                            ListViewItem lvi = new ListViewItem(rev_count.ToString());
                            lvi.SubItems.Add(acc_peak.ToString());
                            lvi.SubItems.Add(base_frequency.ToString());
                            lvi.SubItems.Add(harmonic_peak_0_5.ToString());
                            lvi.SubItems.Add(harmonic_peak_1.ToString());
                            lvi.SubItems.Add(harmonic_peak_1_5.ToString());
                            lvi.SubItems.Add(harmonic_peak_2.ToString());
                            lvi.SubItems.Add(harmonic_peak_2_5.ToString());
                            lvi.SubItems.Add(harmonic_peak_3.ToString());
                            lvi.SubItems.Add(harmonic_peak_4.ToString());
                            lvi.SubItems.Add(harmonic_peak_5.ToString());
                            lvi.SubItems.Add(harmonic_peak_6.ToString());
                            lvi.SubItems.Add(speed_peak.ToString());
                            lvi.SubItems.Add(offset_peak.ToString());
                            lvi.SubItems.Add(temperature.ToString());
                            lvi.SubItems.Add(power.ToString("D"));
                            lvi.SubItems.Add(acc_mean.ToString());
                            lvi.SubItems.Add(acc_min.ToString());
                            lvi.SubItems.Add(displace_min.ToString());

                            lvw_DataValue.Items.Add(lvi);

                            lvw_DataValue.Items[lvw_DataValue.Items.Count - 1].EnsureVisible();
                        }
                    }
                }
                else
                {
                    if (len == (buf[1] + buf[2] * 256))
                    {
                        //byte[] newA=  a.Skip(2).Take(5).ToArray();  

                        switch (buf[5])
                        {
                            case 0x04:
                                {
                                    byte[] confbuf = buf.Skip(6).Take(len - 7).ToArray();
                                    getconf_resp_process(confbuf,(UInt16) confbuf.Length);
                                }
                                break;
                            case 0x81:
                                {
                                    byte[] version = buf.Skip(6).Take(len - 7).ToArray();
                                    getversion_resp_process(version, (UInt16)version.Length);
                                }
                                break;
                            default:break;
                        }

                    }
                }
            }
        }

        public void getconf_resp_process(byte[] buf, UInt16 len)
        {
            UInt16 ptr_count = 0;
            while (true)
            {
                switch (buf[ptr_count])
                {
                    case 0x44:
                    {
                        float mv_to_Acc_p;
                        mv_to_Acc_p = BitConverter.ToSingle(buf, ptr_count + 2);
                        tbx_mvToacc_p.Text = mv_to_Acc_p.ToString();
                        ptr_count += 6; 
                    }
                    break;
                    case 0x45:
                        {
                            if (buf[ptr_count + 2] == 1)
                            {
                                cbx_autoreport.Checked = true;
                            }
                            else if(buf[ptr_count + 2] == 0)
                            {
                                cbx_autoreport.Checked = false;
                            }

                            ptr_count += 3;
                        }
                        break;
                    case 0x46:
                        {
                            byte dis_freq_low;
                            dis_freq_low = buf[ptr_count + 2];
                            tbx_disfreq_low.Text = dis_freq_low.ToString();

                            ptr_count += 3;
                        }
                        break;
                    case 0x47:
                        {
                            byte dis_freq_high;
                            dis_freq_high = buf[ptr_count + 2];
                            tbx_disfreq_high.Text = dis_freq_high.ToString();

                            ptr_count += 3;
                        }
                        break;
                    default:break;
                }

                if (ptr_count >= len - 1)
                {
                    break;
                }
            }
 
            
        }
        public void getversion_resp_process(byte[] buf, UInt16 len)
        {
            lab_version.Text = buf[2].ToString("D") + "." + buf[1].ToString("D") + "." + buf[0].ToString("D");
        }


        private void btn_clear_Click(object sender, EventArgs e)
        {
            tbx_revdata.Text = "";
            lab_version.Text = "-.-.-";
            tbx_mvToacc_p.Text = "";
            lvw_DataValue.Items.Clear();
        }

        private void btn_SetConf_Click(object sender, EventArgs e)
        {

            if (tbx_disfreq_high.Text == "" || tbx_disfreq_low.Text == "" || tbx_mvToacc_p.Text == "")
            {
                MessageBox.Show("有空值");
                return;
            }



            // head len inf cmd tlv foot sum
            /*
             * typedef struct
                {
                    uint8_t head;
                    uint16_t len;
                    uint16_t inf;
                    uint8_t cmd;
                    uint8_t *payload;
                    uint8_t foot;
                    uint8_t sum;
                }ln_protocolintance_t;
             */

            byte[] float_temp = new byte[4];
            byte[] setconf_buf = new byte[100];
            byte dis_freq_low;
            byte dis_freq_high;
            float mv_to_acc_p = 0.0f;

            dis_freq_low = Convert.ToByte(tbx_disfreq_low.Text.ToString());
            dis_freq_high = Convert.ToByte(tbx_disfreq_high.Text.ToString());

            setconf_buf[0] = 0x7E;

            setconf_buf[1] = 23;
            setconf_buf[2] = 0;

            setconf_buf[3] = 0;
            setconf_buf[4] = 0;


            setconf_buf[5] = 0x01;
            setconf_buf[6] = 0x44;
            setconf_buf[7] = 0x04;

            mv_to_acc_p = Convert.ToSingle(tbx_mvToacc_p.Text.ToString());
            float_temp = BitConverter.GetBytes(mv_to_acc_p);
            setconf_buf[8] = float_temp[0];
            setconf_buf[9] = float_temp[1];
            setconf_buf[10] = float_temp[2];
            setconf_buf[11] = float_temp[3];

            setconf_buf[12] = 0x45;
            setconf_buf[13] = 0x01;
            if (cbx_autoreport.Checked == true)
            {
                setconf_buf[14] = 1;
            }
            else 
            {
                setconf_buf[14] = 0;
            }

            setconf_buf[15] = 0x46;
            setconf_buf[16] = 0x01;
            setconf_buf[17] = dis_freq_low;

            setconf_buf[18] = 0x47;
            setconf_buf[19] = 0x01;
            setconf_buf[20] = dis_freq_high;


            setconf_buf[21] = 0x7E;

            byte check_sum = 0;

            for (uint i = 0; i < 22; i++)
            {
                check_sum += setconf_buf[i];
            }
            setconf_buf[22] = check_sum;


            _serialPort.Write( setconf_buf,0,23);

        }

        private void btn_GetConf_Click(object sender, EventArgs e)
        {
            byte[] setconf_buf = new byte[100];
            setconf_buf[0] = 0x7E;

            setconf_buf[1] = 8;
            setconf_buf[2] = 0;

            setconf_buf[3] = 0;
            setconf_buf[4] = 0;


            setconf_buf[5] = 0x03; //cmd

            setconf_buf[6] = 0x7E;

            byte check_sum = 0;

            for (uint i = 0; i < 7; i++)
            {
                check_sum += setconf_buf[i];
            }
            setconf_buf[7] = check_sum;


            _serialPort.Write(setconf_buf, 0, 8);
        }

        private void btn_getversion_Click(object sender, EventArgs e)
        {
            byte[] setconf_buf = new byte[100];
            setconf_buf[0] = 0x7E;

            setconf_buf[1] = 8;
            setconf_buf[2] = 0;

            setconf_buf[3] = 0;
            setconf_buf[4] = 0;


            setconf_buf[5] = 0x80; //cmd

            setconf_buf[6] = 0x7E;

            byte check_sum = 0;

            for (uint i = 0; i < 7; i++)
            {
                check_sum += setconf_buf[i];
            }
            setconf_buf[7] = check_sum;


            _serialPort.Write(setconf_buf, 0, 8);
        }

        private void Serial_Conf_Enter(object sender, EventArgs e)
        {

        }

        private void btn_get_Click(object sender, EventArgs e)
        {
            byte[] setconf_buf = new byte[100];
            setconf_buf[0] = 0x7E;

            setconf_buf[1] = 8;
            setconf_buf[2] = 0;

            setconf_buf[3] = 0;
            setconf_buf[4] = 0;


            setconf_buf[5] = 0xD0; //cmd

            setconf_buf[6] = 0x7E;

            byte check_sum = 0;

            for (uint i = 0; i < 7; i++)
            {
                check_sum += setconf_buf[i];
            }
            setconf_buf[7] = check_sum;


            _serialPort.Write(setconf_buf, 0, 8);
        }

        private void check_sqlite()
        {
            string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory);
            //string path = "E://holiday.json";

            string sql_path;
            sql_path = path + "\\sql.db";

            //判断文件的存在
            if (System.IO.File.Exists(sql_path))
            {
                //存在文件
            }
            else
            {
                //不存在文件
                SQLiteConnection.CreateFile(sql_path);

                sql_path = "data source=" + path + "\\sql.db";
                using (SQLiteConnection conn = new SQLiteConnection(sql_path))
                {


                    using (SQLiteCommand cmd = new SQLiteCommand())
                    {
                        cmd.Connection = conn;
                        conn.Open();

                        SQLiteHelper sh = new SQLiteHelper(cmd);

                        SQLiteTable tb = new SQLiteTable("normal_data");
                        tb.Columns.Add(new SQLiteColumn("id", true));
                        tb.Columns.Add(new SQLiteColumn("time"));
                        tb.Columns.Add(new SQLiteColumn("acc_uppeak"));
                        tb.Columns.Add(new SQLiteColumn("acc_downpeak"));
                        tb.Columns.Add(new SQLiteColumn("acc_mean"));
                        tb.Columns.Add(new SQLiteColumn("acc_05"));
                        tb.Columns.Add(new SQLiteColumn("acc_1"));
                        tb.Columns.Add(new SQLiteColumn("acc_15"));
                        tb.Columns.Add(new SQLiteColumn("base_freq"));
                        tb.Columns.Add(new SQLiteColumn("speedRMS"));
                        tb.Columns.Add(new SQLiteColumn("dis_uppeak"));
                        tb.Columns.Add(new SQLiteColumn("dis_downpeak"));
                        tb.Columns.Add(new SQLiteColumn("time_dif"));
                        sh.CreateTable(tb);

                        SQLiteTable tb2 = new SQLiteTable("abnormal_data");
                        tb2.Columns.Add(new SQLiteColumn("id", true));
                        tb2.Columns.Add(new SQLiteColumn("time"));
                        tb2.Columns.Add(new SQLiteColumn("acc_max"));
                        tb2.Columns.Add(new SQLiteColumn("base_freq"));
                        tb2.Columns.Add(new SQLiteColumn("speed"));
                        tb2.Columns.Add(new SQLiteColumn("displacement"));
                        tb2.Columns.Add(new SQLiteColumn("time_dif"));
                        sh.CreateTable(tb2);

                        conn.Close();
                    }
                }

            }
        }
    }
}
