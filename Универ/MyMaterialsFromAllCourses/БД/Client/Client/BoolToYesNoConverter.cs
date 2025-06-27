using System;
using System.Globalization;
using Microsoft.Maui.Controls;

namespace CinemaClientService.Converters
{
    public class BoolToYesNoConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool boolValue)
                return boolValue ? "Да" : "Нет";
            return "Нет";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is string str)
                return str.Equals("Да", StringComparison.OrdinalIgnoreCase);
            return false;
        }
    }
}