using System.Collections.ObjectModel;
using Microsoft.Maui.Controls;
using CinemaClientService.ViewModels;

namespace CinemaClientService
{
    public partial class SelectMoviePage : ContentPage
    {
        public event EventHandler<MovieView>? MovieSelected;
        private ObservableCollection<MovieView> _allMovies;

        public SelectMoviePage(ObservableCollection<MovieView> movies)
        {
            InitializeComponent();
            _allMovies = new ObservableCollection<MovieView>(movies);
            MoviesCollectionView.ItemsSource = _allMovies;
        }

        private void OnSearchBarTextChanged(object sender, TextChangedEventArgs e)
        {
            var keyword = e.NewTextValue.ToLower();
            MoviesCollectionView.ItemsSource = string.IsNullOrWhiteSpace(keyword)
                ? _allMovies
                : new ObservableCollection<MovieView>(_allMovies.Where(m => m.Name.ToLower().Contains(keyword)));
        }

        private async void OnMovieSelected(object sender, SelectionChangedEventArgs e)
        {
            if (e.CurrentSelection.FirstOrDefault() is MovieView selectedMovie)
            {
                MovieSelected?.Invoke(this, selectedMovie);
                await Navigation.PopAsync();
            }
        }
    }
}