package swing.imageProcessing;

import javax.swing.*;
import java.awt.*;

/**
 * This program demonstrates various image processing operations.
 * @version 1.05 2018-05-01
 * @author Cay Horstmann
 */
public class ImageProcessingTest
{
   public static void main(String[] args)
   {
      EventQueue.invokeLater(() ->
         {
            var frame = new ImageProcessingFrame();
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setVisible(true);
         });
   }
}
