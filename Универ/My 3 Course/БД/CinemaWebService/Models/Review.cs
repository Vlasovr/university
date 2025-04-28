using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

namespace CinemaWebService.Models
{
    public class Review
    {
        [BsonId] // Указывает, что это поле является идентификатором документа
        [BsonRepresentation(BsonType.ObjectId)] // Позволяет использовать строковое представление ObjectId
        public string? Id { get; set; } // Свойство для хранения _id из MongoDB опционально для него, для posgres оно должно неопциональным
        public int ReviewId { get; set; } // PK
        public int Rating { get; set; }
        public string Comment { get; set; }
        public string Source { get; set; }

        // FK
        public int MovieId { get; set; }
        public int ClientId { get; set; }
    }
}