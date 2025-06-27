using System.Collections.ObjectModel;
using System.Linq;
using System.Net.Http.Json;
using Microsoft.Maui.Controls;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using CinemaClientService.ViewModels;

namespace CinemaClientService
{
    public partial class SessionEmployeesPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        public ObservableCollection<SessionEmployeeView> SessionEmployees { get; set; }
        private bool isLoading;

        public bool IsLoading
        {
            get => isLoading;
            set
            {
                isLoading = value;
                OnPropertyChanged();
            }
        }

        public SessionEmployeeView SelectedSessionEmployee { get; set; }

        public SessionEmployeesPage(HttpClient httpClient)
        {
            InitializeComponent();
            _httpClient = httpClient;
            SessionEmployees = new ObservableCollection<SessionEmployeeView>();
            SessionEmployeeList.ItemsSource = SessionEmployees;
            BindingContext = this;
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadSessionEmployees();
        }

        private async Task LoadSessionEmployees()
        {
            try
            {
                IsLoading = true;
                SessionEmployees.Clear();

                var sessionEmployees = await _httpClient.GetFromJsonAsync<List<SessionEmployeeView>>("api/SessionEmployees");

                if (sessionEmployees != null)
                {
                    foreach (var se in sessionEmployees)
                    {
                        SessionEmployees.Add(se);
                    }
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Не удалось загрузить данные: {ex.Message}", "OK");
            }
            finally
            {
                IsLoading = false;
            }
        }

        private async void OnSearchBarTextChanged(object sender, TextChangedEventArgs e)
        {
            string query = e.NewTextValue.ToLower();
            if (string.IsNullOrEmpty(query))
            {
                SessionEmployees.Clear();
                await LoadSessionEmployees();
            }
            else
            {
                // Попытка преобразовать запрос в целое число для поиска SessionId или EmployeeId
                bool isSessionId = int.TryParse(query, out int sessionId);
                bool isEmployeeId = int.TryParse(query, out int employeeId);

                var filtered = SessionEmployees.Where(se =>
                                    (isSessionId && se.SessionId.ToString().Contains(query)) ||
                                    (isEmployeeId && se.EmployeeId.ToString().Contains(query)) ||
                                    se.SessionInfo.ToLower().Contains(query) ||
                                    se.EmployeeInfo.ToLower().Contains(query))
                                      .ToList();
                SessionEmployeeList.ItemsSource = filtered;
            }
        }

        private async void OnAddSessionEmployeeClicked(object sender, EventArgs e)
        {
            var form = new SessionEmployeeFormPage(_httpClient, null);
            await Navigation.PushAsync(form);
        }

        private async void OnDeleteSessionEmployeeClicked(object sender, EventArgs e)
        {
            if (SelectedSessionEmployee != null)
            {
                bool confirm = await DisplayAlert("Удаление",
                    $"Вы уверены, что хотите удалить связь Session ID {SelectedSessionEmployee.SessionId} и Employee ID {SelectedSessionEmployee.EmployeeId}?",
                    "Да", "Нет");
                if (confirm)
                {
                    try
                    {
                        var response = await _httpClient.DeleteAsync($"api/SessionEmployees/{SelectedSessionEmployee.SessionId}/{SelectedSessionEmployee.EmployeeId}");
                        if (response.IsSuccessStatusCode)
                        {
                            SessionEmployees.Remove(SelectedSessionEmployee);
                            SelectedSessionEmployee = null;
                        }
                        else
                        {
                            var content = await response.Content.ReadAsStringAsync();
                            await DisplayAlert("Ошибка",
                                $"Не удалось удалить связь (код {response.StatusCode}): {content}",
                                "OK");
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Ошибка", $"Не удалось удалить связь: {ex.Message}", "OK");
                    }
                }
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите связь для удаления", "OK");
            }
        }

        private void OnSessionEmployeeSelected(object sender, SelectionChangedEventArgs e)
        {
            SelectedSessionEmployee = e.CurrentSelection.FirstOrDefault() as SessionEmployeeView;
        }

        // Реализация интерфейса INotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected new void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}