using System;
using System.Collections.ObjectModel;
using System.Linq;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels;

namespace CinemaClientService
{
    public partial class SelectClientPage : ContentPage
    {
        // Событие, которое будет вызвано при выборе клиента
        public event EventHandler<ClientView>? ClientSelected;

        // Полный список клиентов для поиска
        private ObservableCollection<ClientView> _allClients;

        public SelectClientPage(ObservableCollection<ClientView> clients)
        {
            InitializeComponent();

            // Копируем список клиентов для фильтрации
            _allClients = new ObservableCollection<ClientView>(clients);
            ClientsCollectionView.ItemsSource = _allClients;
        }

        // Обработчик изменения текста в SearchBar для фильтрации списка клиентов
        private void OnSearchBarTextChanged(object sender, TextChangedEventArgs e)
        {
            var keyword = e.NewTextValue?.ToLower() ?? string.Empty;

            if (string.IsNullOrWhiteSpace(keyword))
            {
                // Если строка поиска пуста, отображаем полный список
                ClientsCollectionView.ItemsSource = _allClients;
            }
            else
            {
                // Фильтруем список клиентов по имени
                var filteredClients = _allClients.Where(c => c.FullName.ToLower().Contains(keyword));
                ClientsCollectionView.ItemsSource = new ObservableCollection<ClientView>(filteredClients);
            }
        }

        // Обработчик выбора клиента из списка
        private async void OnClientSelected(object sender, SelectionChangedEventArgs e)
        {
            if (e.CurrentSelection.FirstOrDefault() is ClientView selectedClient)
            {
                // Вызываем событие выбора клиента
                ClientSelected?.Invoke(this, selectedClient);

                // Возвращаемся на предыдущую страницу
                await Navigation.PopAsync();
            }
        }
    }
}