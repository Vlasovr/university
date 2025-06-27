using System.Collections.ObjectModel;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels;

namespace CinemaClientService
{
    public partial class SelectEmployeePage : ContentPage
    {
        public event EventHandler<EmployeeView> EmployeeSelected;

        private readonly ObservableCollection<EmployeeView> _employees;

        public SelectEmployeePage(IEnumerable<EmployeeView> employees)
        {
            InitializeComponent();
            _employees = new ObservableCollection<EmployeeView>(employees);
            EmployeeList.ItemsSource = _employees;
        }

        private async void OnEmployeeSelected(object sender, SelectionChangedEventArgs e)
        {
            var selectedEmployee = e.CurrentSelection.FirstOrDefault() as EmployeeView;
            if (selectedEmployee != null)
            {
                EmployeeSelected?.Invoke(this, selectedEmployee);
                await Navigation.PopAsync();
            }
        }
    }
}