using System.Collections.ObjectModel;
using System.Net.Http;
using System.Net.Http.Json;
using Microsoft.Maui.Controls;

namespace CinemaClientService
{
    public partial class MainPage : ContentPage
    {
        private readonly HttpClient _httpClient;
        public ObservableCollection<EmployeeView> Employees { get; set; }

        public MainPage(HttpClient httpClient)
        {
            InitializeComponent();
            _httpClient = httpClient;
            Employees = new ObservableCollection<EmployeeView>();
            EmployeeList.ItemsSource = Employees;
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadEmployees();
        }

        private async Task LoadEmployees()
        {
            try
            {
                var employees = await _httpClient.GetFromJsonAsync<List<EmployeeView>>("api/Employees");
                Employees.Clear();
                foreach (var emp in employees)
                {
                    Employees.Add(emp);
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Не удалось загрузить данные: {ex.Message}", "ОК");
            }
        }

        private async void OnSearchBarTextChanged(object sender, TextChangedEventArgs e)
        {
            string query = e.NewTextValue;
            if (string.IsNullOrEmpty(query))
            {
                await LoadEmployees();
            }
            else
            {
                var filtered = Employees.Where(emp => emp.FullName.Contains(query, StringComparison.OrdinalIgnoreCase)).ToList();
                EmployeeList.ItemsSource = filtered;
            }
        }

        private async void OnAddEmployeeClicked(object sender, EventArgs e)
        {
            var form = new EmployeeFormPage(_httpClient, null);
            await Navigation.PushAsync(form);
        }

        private async void OnEditEmployeeClicked(object sender, EventArgs e)
        {
            if (EmployeeList.SelectedItem is EmployeeView selectedEmployee)
            {
                var form = new EmployeeFormPage(_httpClient, selectedEmployee);
                await Navigation.PushAsync(form);
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите сотрудника для редактирования", "ОК");
            }
        }

        private async void OnDeleteEmployeeClicked(object sender, EventArgs e)
        {
            if (EmployeeList.SelectedItem is EmployeeView selectedEmployee)
            {
                bool confirm = await DisplayAlert("Удаление", $"Вы уверены, что хотите удалить {selectedEmployee.FullName}?", "Да", "Нет");
                if (confirm)
                {
                    try
                    {
                        await _httpClient.DeleteAsync($"api/Employees/{selectedEmployee.EmployeeId}");
                        Employees.Remove(selectedEmployee);
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Ошибка", $"Не удалось удалить: {ex.Message}", "ОК");
                    }
                }
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите сотрудника для удаления", "ОК");
            }
        }

        private void OnEmployeeSelected(object sender, SelectionChangedEventArgs e)
        {
            // Логика выбора сотрудника, если нужна
        }
    }

    public class EmployeeView
    {
        public int EmployeeId { get; set; }
        public string FullName { get; set; }
        public string Phone { get; set; }
        public string Position { get; set; }

        // Новые поля
        public string Email { get; set; }
        public string Address { get; set; }
    }
}