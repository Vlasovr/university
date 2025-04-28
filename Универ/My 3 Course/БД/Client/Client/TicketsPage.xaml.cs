using System.Collections.ObjectModel;
using System.Linq;
using System.Net.Http.Json;
using Microsoft.Maui.Controls;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using CinemaClientService.ViewModels;

namespace CinemaClientService
{
    public partial class TicketsPage : ContentPage, INotifyPropertyChanged
    {
        private readonly HttpClient _httpClient;
        public ObservableCollection<TicketView> Tickets { get; set; }
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

        public TicketView SelectedTicket { get; set; }

        public TicketsPage(HttpClient httpClient)
        {
            InitializeComponent();
            _httpClient = httpClient;
            Tickets = new ObservableCollection<TicketView>();
            TicketList.ItemsSource = Tickets;
            BindingContext = this;
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await LoadTickets();
        }

        private async Task LoadTickets()
        {
            try
            {
                IsLoading = true;
                Tickets.Clear();
                
                var tickets = await _httpClient.GetFromJsonAsync<List<TicketView>>("api/Tickets");

                if (tickets != null)
                {
                    foreach (var ticket in tickets)
                    {
                        Tickets.Add(ticket);
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
                Tickets.Clear();
                await LoadTickets();
            }
            else
            {
                bool isClientId = int.TryParse(query, out int clientId);
                bool isSeatId = int.TryParse(query, out int seatId);
                bool isSessionId = int.TryParse(query, out int sessionId);

                var filtered = Tickets.Where(t =>
                                    (isClientId && t.ClientId.ToString().Contains(query)) ||
                                    (isSeatId && t.SeatId.ToString().Contains(query)) ||
                                    (isSessionId && t.SessionId.ToString().Contains(query)) ||
                                    t.Category.ToLower().Contains(query))
                                      .ToList();
                TicketList.ItemsSource = filtered;
            }
        }

        private async void OnAddTicketClicked(object sender, EventArgs e)
        {
            var form = new TicketFormPage(_httpClient, null);
            await Navigation.PushAsync(form);
        }

        private async void OnEditTicketClicked(object sender, EventArgs e)
        {
            if (SelectedTicket != null)
            {
                var form = new TicketFormPage(_httpClient, SelectedTicket);
                await Navigation.PushAsync(form);
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите билет для редактирования", "OK");
            }
        }

        private async void OnDeleteTicketClicked(object sender, EventArgs e)
        {
            if (SelectedTicket != null)
            {
                bool confirm = await DisplayAlert("Удаление",
                    $"Вы уверены, что хотите удалить билет ID {SelectedTicket.TicketId}?",
                    "Да", "Нет");
                if (confirm)
                {
                    try
                    {
                        var response = await _httpClient.DeleteAsync($"api/Tickets/{SelectedTicket.TicketId}");
                        if (response.IsSuccessStatusCode)
                        {
                            Tickets.Remove(SelectedTicket);
                            SelectedTicket = null;
                        }
                        else
                        {
                            var content = await response.Content.ReadAsStringAsync();
                            await DisplayAlert("Ошибка",
                                $"Не удалось удалить билет (код {response.StatusCode}): {content}",
                                "OK");
                        }
                    }
                    catch (Exception ex)
                    {
                        await DisplayAlert("Ошибка", $"Не удалось удалить билет: {ex.Message}", "OK");
                    }
                }
            }
            else
            {
                await DisplayAlert("Ошибка", "Выберите билет для удаления", "OK");
            }
        }

        private void OnTicketSelected(object sender, SelectionChangedEventArgs e)
        {
            SelectedTicket = e.CurrentSelection.FirstOrDefault() as TicketView;
        }

        public new event PropertyChangedEventHandler PropertyChanged;

        protected new void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}