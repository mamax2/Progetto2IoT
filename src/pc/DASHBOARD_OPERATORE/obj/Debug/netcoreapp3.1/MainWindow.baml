   M S B A M L   `   `   `  ����N  JDASHBOARD_OPERATORE, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null9!clr-namespace:DASHBOARD_OPERATOREDASHBOARD_OPERATORE  5   	   $    DASHBOARD_OPERATORE.MainWindow   5   	   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35e aSystem.Windows.Controls.Ribbon, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   3local!clr-namespace:DASHBOARD_OPERATORE     5   	     � 
ResizeMode$  CanMinimize=�5   	    � Title$ DASHBOARD OPERATORE��5	   	   $	��342��6%   $	��534��62    � 	Resources 5         
FlatButton �� 
TargetType �� Property �� Value �� Triggers P� Property P� Value��%�  &	        �� 5   
   " ��6#   .�5      ��  � �� �#00796B6+   �� 5       � ��
 �����6+   �� 5       � ��
 ���� 6,   �� 5       � ��$ 16(�6)   �� 5       � ��$
 10,5q�6(   �� 5       �@�Cursor$
 Hand��6'    5      P� 5       �@J�IsMouseOver$
 True��61   . �5      ��  � �� �#00B38663   �� 5       � ��
 �   �63   5      5      5      .��5      � .+�5    
   �� -   5        	 ��Name$	 btnEmpty��5       #��[  6#   $��Empty��6G   $
��Left=�6W   $��
22,238,0,0q�6r   $	��Top=�6�   �� 5!   
   -   5        $	 
btnRestore��5!      #��[  6%   $��Restore��6I   $
��Left=�6[   $��423,238,0,0q�6v   $	��Top=�6�   �� 5"   
   $,��&Monitoraggio Riempimento e Temperatura��6   $��16(�6C   $��10q�6Q   $��20,20,36,125q�6^   .��5#      �� .+�5&      �� $��Livello di Riempimento:��6   $��14(�6;   $��0,0,0,5q�6I   � 5'      -   5        $��FillLevelProgressBar��5'      
 � Minimum$
 0Q�6:    � Maximum$	 100Q�6F   $��20��6T   $	��200��6`    � Value$ 50Q�6l   �� 5(      -   5        $��FillLevelText��5(      $	��50%��61   $��12(�6<   $��Center=�6J   $��0,5,0,15q�6g   �� 5+      $��Temperatura:��6   $��14(�60   $��0,0,0,5q�6>   �� 5,      -   5        $��TemperatureText��5,      $��25°C��63   $��12(�6?   $��Center=�6M   $��0,5,0,15q�6j   5-      5.      5/      50      