using System.Collections.ObjectModel;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Net.Http.Json;

namespace CinemaClientService
{
    public partial class SessionEmployeeFormPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        private readonly SessionEmployeeView? _editingSessionEmployee;

        private ObservableCollection<SessionView> Sessions { get; set; }
        private ObservableCollection<EmployeeView> Employees { get; set; }

        private SessionView? _selectedSession;
        private EmployeeView? _selectedEmployee;

        private bool _isAdding;
        public bool IsAdding
        {
            get => _isAdding;
            set
            {
                _isAdding = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(IsEditing));
            }
        }

        public bool IsEditing => !IsAdding;

        public string PageTitle => _editingSessionEmployee == null ? "Добавить связь Сессия-Сотрудник" : "Редактировать связь Сессия-Сотрудник";

        public SessionEmployeeFormPage(HttpClient httpClient, SessionEmployeeView? sessionEmployee)
        {
            InitializeComponent();
            _httpClient = httpClient;
            _editingSessionEmployee = sessionEmployee;

            Sessions = new ObservableCollection<SessionView>();
            Employees = new ObservableCollection<EmployeeView>();

            BindingContext = this;

            if (_editingSessionEmployee != null)
            {
                // Режим редактирования
                IsAdding = false;
                SessionIdLabel.Text = _editingSessionEmployee.SessionId.ToString();
                SelectedSessionInfoLabel.Text = _editingSessionEmployee.SessionInfo;
                EmployeeIdLabel.Text = _editingSessionEmployee.EmployeeId.ToString();
                SelectedEmployeeLabel.Text = _editingSessionEmployee.EmployeeInfo;

                _selectedSession = new SessionView { SessionId = _editingSessionEmployee.SessionId };
                _selectedEmployee = new EmployeeView { EmployeeId = _editingSessionEmployee.EmployeeId };
            }
            else
            {
                // Режим добавления
                IsAdding = true;
                SessionIdLabel.Text = "Не выбрано";
            }
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadSessions();
            await LoadEmployees();
        }

        private async Task LoadSessions()
        {
            try
            {
                var sessions = await _httpClient.GetFromJsonAsync<List<SessionView>>("api/Sessions");
                Sessions.Clear();
                if (sessions != null)
                {
                    foreach (var session in sessions)
                    {
                        Sessions.Add(session);
                    }
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Не удалось загрузить сессии: {ex.Message}", "OK");
            }
        }

        private async Task LoadEmployees()
        {
            try
            {
                var employees = await _httpClient.GetFromJsonAsync<List<EmployeeView>>("api/Employees");
                Employees.Clear();
                if (employees != null)
                {
                    foreach (var employee in employees)
                    {
                        Employees.Add(employee);
                    }
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Не удалось загрузить сотрудников: {ex.Message}", "OK");
            }
        }

        private async void OnSelectSessionClicked(object sender, EventArgs e)
        {
            // Открываем страницу выбора сессии
            var selectSessionPage = new SelectSessionPage(Sessions);
            selectSessionPage.SessionSelected += (s, session) =>
            {
                _selectedSession = session;
                // Отображаем информативные данные о сессии
                SessionIdLabel.Text = session.SessionId.ToString();
                SelectedSessionLabel.Text = $"{session.StartTime:dd.MM.yyyy HH:mm} - {session.Format}";
            };
            await Navigation.PushAsync(selectSessionPage);
        }

        private async void OnSelectEmployeeClicked(object sender, EventArgs e)
        {
            // Открываем страницу выбора сотрудника
            var selectEmployeePage = new SelectEmployeePage(Employees);
            selectEmployeePage.EmployeeSelected += (s, employee) =>
            {
                _selectedEmployee = employee;
                SelectedEmployeeLabel.Text = employee.FullName;
            };
            await Navigation.PushAsync(selectEmployeePage);
        }

        private async void OnSaveClicked(object sender, EventArgs e)
        {
            // Валидация
            if (_selectedSession == null)
            {
                await DisplayAlert("Ошибка", "Пожалуйста, выберите сессию.", "OK");
                return;
            }

            if (_selectedEmployee == null)
            {
                await DisplayAlert("Ошибка", "Пожалуйста, выберите сотрудника.", "OK");
                return;
            }

            var sessionEmployee = new SessionEmployeeView
            {
                SessionId = _selectedSession.SessionId,
                EmployeeId = _selectedEmployee.EmployeeId
            };

            try
            {
                if (_editingSessionEmployee == null)
                {
                    // Добавление новой связи
                    var response = await _httpClient.PostAsJsonAsync("api/SessionEmployees", sessionEmployee);
                    if (response.IsSuccessStatusCode)
                    {
                        await DisplayAlert("Успех", "Связь успешно добавлена.", "OK");
                        await Navigation.PopAsync();
                    }
                    else
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось добавить связь (код {response.StatusCode}): {content}", "OK");
                    }
                }
                else
                {
                    // Редактирование существующей связи (не рекомендуется для связующих таблиц с составным ключом)
                    await DisplayAlert("Информация", "Редактирование связей не поддерживается. Пожалуйста, удалите и создайте новую связь.", "OK");
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Произошла ошибка: {ex.Message}", "OK");
            }
        }

        private async void OnCancelClicked(object sender, EventArgs e)
        {
            await Navigation.PopAsync();
        }

        // Реализация интерфейса INotifyPropertyChanged
        public new event PropertyChangedEventHandler PropertyChanged;

        protected new void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    namespace CinemaClientService.ViewModels
    {
        public class SessionView
        {
            public int SessionId { get; set; }
            public DateTime StartTime { get; set; }
            public DateTime EndTime { get; set; }
            public int AvailableSeats { get; set; }
            public string Format { get; set; } = string.Empty;

            // Связанные данные
            public int HallId { get; set; }

            // Дополнительное свойство для отображения информации о сессии
            public string SessionInfo => $"{StartTime:dd.MM.yyyy HH:mm} - {Format}";
        }

        public class EmployeeView
        {
            public int EmployeeId { get; set; }
            public string FullName { get; set; } = string.Empty;
            // Добавьте другие свойства сотрудника, если необходимо
        }
    }
}