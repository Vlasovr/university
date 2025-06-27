using System.Collections.ObjectModel;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Net.Http.Json;

namespace CinemaClientService
{
    public partial class TicketFormPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        private readonly TicketView? _editingTicket;

        private ObservableCollection<ClientView> Clients { get; set; }
        private ObservableCollection<SeatView> Seats { get; set; }
        private ObservableCollection<SessionView> Sessions { get; set; }

        private ClientView? _selectedClient;
        private SeatView? _selectedSeat;
        private SessionView? _selectedSession;

        public string PageTitle => _editingTicket == null ? "Добавить билет" : "Редактировать билет";

        public TicketFormPage(HttpClient httpClient, TicketView? ticket)
        {
            InitializeComponent();
            _httpClient = httpClient;
            _editingTicket = ticket;

            Clients = new ObservableCollection<ClientView>();
            Seats = new ObservableCollection<SeatView>();
            Sessions = new ObservableCollection<SessionView>();

            BindingContext = this;

            if (_editingTicket != null)
            {
                // Режим редактирования
                TicketIdEntry.Text = _editingTicket.TicketId.ToString();
                SelectedClientLabel.Text = _editingTicket.ClientId.ToString();
                SelectedSeatLabel.Text = _editingTicket.SeatId.ToString();
                SelectedSessionLabel.Text = _editingTicket.SessionId.ToString();
                PriceEntry.Text = _editingTicket.Price.ToString();
                CategoryEntry.Text = _editingTicket.Category;
                PurchaseDatePicker.Date = _editingTicket.PurchaseTime.Date;
                PurchaseTimePicker.Time = _editingTicket.PurchaseTime.TimeOfDay;

                _selectedClient = new ClientView { ClientId = _editingTicket.ClientId };
                _selectedSeat = new SeatView { SeatId = _editingTicket.SeatId };
                _selectedSession = new SessionView { SessionId = _editingTicket.SessionId };
            }
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadClients();
            await LoadSeats();
            await LoadSessions();
        }

        private async Task LoadClients()
        {
            try
            {
                var clients = await _httpClient.GetFromJsonAsync<List<ClientView>>("api/Clients");
                Clients.Clear();
                if (clients != null)
                {
                    foreach (var client in clients)
                    {
                        Clients.Add(client);
                    }
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Не удалось загрузить клиентов: {ex.Message}", "OK");
            }
        }

        private async Task LoadSeats()
        {
            try
            {
                var seats = await _httpClient.GetFromJsonAsync<List<SeatView>>("api/Seats");
                Seats.Clear();
                if (seats != null)
                {
                    foreach (var seat in seats)
                    {
                        Seats.Add(seat);
                    }
                }
            }
            catch (Exception ex)
            {
                await DisplayAlert("Ошибка", $"Не удалось загрузить места: {ex.Message}", "OK");
            }
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
                await DisplayAlert("Ошибка", $"Не удалось загрузить сеансы: {ex.Message}", "OK");
            }
        }

        private async void OnSelectClientClicked(object sender, EventArgs e)
        {
            // Открываем страницу выбора клиента
            var selectClientPage = new SelectClientPage(Clients);
            selectClientPage.ClientSelected += (s, client) =>
            {
                _selectedClient = client;
                SelectedClientLabel.Text = client.ClientId.ToString();
            };
            await Navigation.PushAsync(selectClientPage);
        }

        private async void OnSelectSeatClicked(object sender, EventArgs e)
        {
            // Открываем страницу выбора места
            var selectSeatPage = new SelectSeatPage(Seats);
            selectSeatPage.SeatSelected += (s, seat) =>
            {
                _selectedSeat = seat;
                SelectedSeatLabel.Text = seat.SeatId.ToString();
            };
            await Navigation.PushAsync(selectSeatPage);
        }

        private async void OnSelectSessionClicked(object sender, EventArgs e)
        {
            // Открываем страницу выбора сессии
            var selectSessionPage = new SelectSessionPage(Sessions);
            selectSessionPage.SessionSelected += (s, session) =>
            {
                _selectedSession = session;
                SelectedSessionLabel.Text = session.SessionId.ToString();
            };
            await Navigation.PushAsync(selectSessionPage);
        }

        private async void OnSaveClicked(object sender, EventArgs e)
        {
            // Валидация
            if (_selectedClient == null)
            {
                await DisplayAlert("Ошибка", "Пожалуйста, выберите клиента.", "OK");
                return;
            }

            if (_selectedSeat == null)
            {
                await DisplayAlert("Ошибка", "Пожалуйста, выберите место.", "OK");
                return;
            }

            if (_selectedSession == null)
            {
                await DisplayAlert("Ошибка", "Пожалуйста, выберите сессию.", "OK");
                return;
            }

            if (!decimal.TryParse(PriceEntry.Text, out decimal price) || price < 0)
            {
                await DisplayAlert("Ошибка", "Цена должна быть неотрицательным числом.", "OK");
                return;
            }

            if (string.IsNullOrWhiteSpace(CategoryEntry.Text))
            {
                await DisplayAlert("Ошибка", "Пожалуйста, введите категорию билета.", "OK");
                return;
            }

            var purchaseDateTimeLocal = PurchaseDatePicker.Date + PurchaseTimePicker.Time;
            var purchaseDateTimeUtc = purchaseDateTimeLocal.ToUniversalTime();

            var ticket = new TicketView
            {
                ClientId = _selectedClient.ClientId,
                SeatId = _selectedSeat.SeatId,
                SessionId = _selectedSession.SessionId,
                Price = price,
                Category = CategoryEntry.Text,
                PurchaseTime = purchaseDateTimeUtc
            };

            try
            {
                if (_editingTicket == null)
                {
                    // Добавление нового билета
                    var response = await _httpClient.PostAsJsonAsync("api/Tickets", ticket);
                    if (response.IsSuccessStatusCode)
                    {
                        await DisplayAlert("Успех", "Билет успешно добавлен.", "OK");
                        await Navigation.PopAsync();
                    }
                    else
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось добавить билет (код {response.StatusCode}): {content}", "OK");
                    }
                }
                else
                {
                    // Редактирование существующего билета
                    ticket.TicketId = _editingTicket.TicketId;
                    var response = await _httpClient.PutAsJsonAsync($"api/Tickets/{ticket.TicketId}", ticket);
                    if (response.IsSuccessStatusCode)
                    {
                        await DisplayAlert("Успех", "Билет успешно обновлен.", "OK");
                        await Navigation.PopAsync();
                    }
                    else
                    {
                        var content = await response.Content.ReadAsStringAsync();
                        await DisplayAlert("Ошибка", $"Не удалось обновить билет (код {response.StatusCode}): {content}", "OK");
                    }
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
}